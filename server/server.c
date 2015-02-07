/*
 * =====================================================================================
 *
 *       Filename:  server.c
 *
*    Description:  服务器初始化代码模块
 
 *        Version:  1.0
 *        Created:  2015年01月29日 21时25分46秒
 *       Revision:  none
 *       Compiler:  clang 
 *
 *         Author:  magichan (fgm), 574932286@qq.com
 *   Organization:  FH Südwestfalen, Iserlohn
 *           Rule:  ???? question 
 *                  [    comment
 *
 * =====================================================================================*/

#include"server.h"
#define _DEBUG_   

/*   全局变量，所有线程共享  */
PtrUserDate g_user_list;
        int g_sock_fd;



int main( void )
{
        int clie_fd;
        int AllClieFd[CLIENT_MAX] = {-1};//存放所有的套接字
        int Maxfd;//存放最大的文件描述符
        struct SerToCliFrame send_data;
        fd_set readfile,Allreadfile;//select所用 的文件描述符
        pthread_t pthread_slove_connect;
        
        /*用户列表初始化*/
       g_user_list =  InitUserList();
       /*套接字初始化*/
       InitSock();
        
        FD_ZERO(&readfile);
        FD_SET(g_sock_fd,&Allreadfile);
        Maxfd = g_sock_fd;
/*         
 *         遗留问题，主线程无法退出
 *         可能的处理方法：信号
 */
        while(1)//循环等待
        {
                readfile = Allreadfile;
                int flag;
#if(1)
                if( FD_ISSET(g_sock_fd,&readfile))
                {
                        printf("g_sock is on readfile \n");
                }
                if( FD_ISSET(clie_fd,&readfile))
                {
                        printf("clie_fd is on readfile \n");
                }
#endif
//                input_msg("我两次以上\n");
                flag = select(Maxfd+1,&readfile,NULL,NULL,NULL);
//                input_msg("select获取读入\n");

                if( flag <= 0 )
                {
                        MyError("select\n",__FUNCTION__,__LINE__);
                }
                        
                if( flag >= 1 )
                {
                        int i;
                        input_msg("来了几个信息:%d",flag);
                        if( FD_ISSET(g_sock_fd,&readfile) )
                        {
                              input_msg("出现多余一次就有问题\n");
                               struct sockaddr_in  clie_addr;//请求连接者地址
                               socklen_t serv_addr_len;
                               serv_addr_len = sizeof(struct sockaddr_in);
                              clie_fd = accept(g_sock_fd,(struct sockaddr *)&clie_addr,&serv_addr_len);
                              if(clie_fd < 0 )         Log("连接失败","client");
                              else{
                                    printf("Ip : %s 请求连接\n",inet_ntoa(clie_addr.sin_addr));
                                   for( i=0; i<CLIENT_MAX; i++ )
                                   {
                                           if( AllClieFd[i] <= 0 )
                                           {
                                                   AllClieFd[i] = clie_fd;
                                                   break;
                                           }
                                   }//for( i=0; i<CLIENT_MAX;i++),为clie_fd找个新的位置 

    //                               input_msg("检测 第一个 套接字描述符放在那个位置 %d \n",i);
                                   if( i >= CLIENT_MAX ) 
                                   {
                                           memset(&send_data,0,sizeof(struct SerToCliFrame)); 
                                           send_data.option = ERROR_EXCEEDING;
                                           Log("登陆过多，拒绝访问\n",inet_ntoa(clie_addr.sin_addr));
                                           write(clie_fd,&send_data,sizeof(struct SerToCliFrame));
                                           close(clie_fd);
                                   }else{
      //                                     input_msg("套接字%d 成功 放入 Allreadfile",clie_fd);
                                           FD_SET(clie_fd,&Allreadfile);
                                           Maxfd = clie_fd>Maxfd?clie_fd:Maxfd;
                                           Log("请求连接",inet_ntoa(clie_addr.sin_addr));
                                   }//if( i >= CLIENT_MAX ) ，处理超额情况
                              }//if(clie_fd<0)

                        }// if( FD_ISSET(g_sock_fd,&readfile) ) 
                        for( i=0; i< CLIENT_MAX; i++ )
                        {
                                if(FD_ISSET(AllClieFd[i],&readfile))
                                {
                                        input_msg("%d 的端口有信息发送过来\n",AllClieFd[i]);
                                       switch(AnalyzeMesg( AllClieFd[i] ))
                                       {
                                               case 1:
                                                       Log("申请退出",FdToUsername(AllClieFd[i]));
                                                       close(AllClieFd[i]);
                                                       AllClieFd[i] = -1;
                                                       break;
                                               case -1:Log("非法退出","麻烦");
                                                       close(AllClieFd[i]);
                                                       AllClieFd[i] = -1;
                                                       break;
                                               default:
                                                       break;
                                       }//switch 处理客户端关闭，并且重置空间
                                }// if(FD_ISSET(AllClieFd[i],&readfile))
                        }//for( i=0; i< CLIENT_MAX; i++ ),处理有数据发送过来的套接字
                }//if(flag > = 1 )
        }//while(1)



}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  InitSock
 *  Description:  初始化套接字g_sock_fd
 *        Entry:
 *         Exit:
 * =====================================================================================
 */
void  InitSock(void)
{ 
        int optval = 1;
        struct sockaddr_in serv_addr;

       /*创建套接字*/
       if((g_sock_fd = socket(AF_INET,SOCK_STREAM,0)) < 0 )
       {
               MyError("socket",__FUNCTION__,__LINE__);
       }
       /*设置套接字*/ 
       if( setsockopt(g_sock_fd,SOL_SOCKET,SO_REUSEADDR,(void *)&optval,sizeof(int)) < 0 )
       {
               MyError("setsockopt",__FUNCTION__,__LINE__);
       }
       /*初始化地址*/
       memset(&serv_addr,0,sizeof(struct sockaddr_in));
       serv_addr.sin_family = AF_INET;
       serv_addr.sin_port = htons(SERV_PORT);
       serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);//指定地址为0.0.0.0
       /*绑定套接字*/
       if( bind(g_sock_fd,(struct sockaddr * )&serv_addr,sizeof(struct sockaddr_in)) < 0 ) 
       {
               MyError("bind\n",__FUNCTION__,__LINE__);
       }
       /*监视套接字*/
       if( listen(g_sock_fd,LISTENQ) < 0 )
       {
               MyError("listen\n",__FUNCTION__,__LINE__);
       }
}
                                                   
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  AnalyzeMesg
 *  Description:  解析发送过来的数据
 *        Entry:  客户端套接字
 *         Exit:   返回-1 ,表示套接字关闭，无法读取，需要重置该套接字
 *                 返回 1 ，表示申请关闭套接字
 *                 返回 0 ，正常
 * =====================================================================================
 */
int   AnalyzeMesg( int clie_fd )
{
        struct CliToSerFrame get_data;
        struct SerToCliFrame send_data;
        int    count;

        memset(&get_data,0,sizeof(struct CliToSerFrame));
        
        if((count = read(clie_fd,&get_data,sizeof(struct CliToSerFrame))) == 0 )
        {
                return -1;
        }
        memset(&send_data,0,sizeof(struct SerToCliFrame));
/* 
 *    管理员和普通用户的注册问题：一旦管理员用户注册失败，同时还排列这普通用户，就会出问题
 */



        switch( get_data.option)
        {
                case REQUIE_REGISTER:
                        {
                                if( UserListIsEmpty(g_user_list) )
                                {
                                          send_data.option = REGISTER_USERNAME;
                                          strcpy(send_data.mesg_data,"输入用户名[管理员]：");
                                          write(clie_fd,&send_data,sizeof(struct SerToCliFrame));

                                }else{
                                          send_data.option = REGISTER_USERNAME;
                                          strcpy(send_data.mesg_data,"输入用户名：");
                                          write(clie_fd,&send_data,sizeof(struct SerToCliFrame));
                                }//if，判断是否建立root 账号
                        }
                        return 0;
                case REGISTER_USERNAME:
                        {
                                if( SearchUser(g_user_list,get_data.mesg_data) == NULL )
                                {
                                        PtrUserDate temp = (struct UserDate *)malloc(sizeof(struct UserDate));
                                        strcpy(temp->name,get_data.mesg_data);
                                        temp->status = USER_STATUS_DEALING;//待处理的用户结构
                                        temp->confd = clie_fd;
                                        AddUser(g_user_list,temp);//先将待处理的数据结构添加
                                        send_data.option = REGISTER_USERPASSWORD;

                                        strcpy(send_data.mesg_data,"输入密码:");
                                        write(clie_fd,&send_data,sizeof(struct SerToCliFrame));//将密码要求发送
                                }else{
                                        send_data.option = ERROR_USERNAME_EXISTENCE;
                                        Log("申请的姓名重复，拒绝注册","麻烦");
                                        write(clie_fd,&send_data,sizeof(struct SerToCliFrame));
                                }//用户名重复

                        }
                        return 0;
                case REGISTER_USERPASSWORD:
                        {
                                PtrUserDate temp;

                                if((temp = SearchUser(g_user_list,FdToUsername(clie_fd))) == NULL )
                                {
                                        Log("密码验证错误，端口号找不到相映射的数据","server");
                                        send_data.option =  ERROR_TEXT;
                                        strcpy(send_data.mesg_data,"服务器处理错误");
                                        write(clie_fd,&send_data,sizeof(struct SerToCliFrame));
                                }else{
                                       /*信息赋予*/ 
                                        strcpy(temp->password,get_data.mesg_data);
                                        if( g_user_list->next == temp )//判断是否管理员权限
                                        {
                                                temp->authority = CLIENT_STATUS_ROOT;
                                        }else{
                                                temp->authority = CLIENT_STATUS_COMMON;
                                        }
                                        /*向客户端确认，Log文件写入*/
                                        send_data.option = REGISTER_SUCCSE;
                                        write(clie_fd,&send_data,sizeof(struct SerToCliFrame));
                                        Log("注册成功",temp->name);
                                }//if((temp = SearchUser(g_user_list,FdToUsername(clie_fd))) == NULL ) 判断和处理密码

                        }
                        return 0;
                case REQUEST_LOGIN:
                        {
                                send_data.option = LOGIN_USERNAME;
                                strcpy(send_data.mesg_data,"输入您的用户名:");
                                write(clie_fd,&send_data,sizeof(struct SerToCliFrame));
                        }
                        return 0;
                case LOGIN_USERNAME:
                        {
                                PtrUserDate temp;
                                if( (temp=SearchUser(g_user_list,get_data.mesg_data)) == NULL )
                                {
                                        send_data.option = ERROR_USRENAME_NOEXISTENCE;
                                        write(clie_fd,&send_data,sizeof(struct SerToCliFrame));
                                }else{
                                        temp->confd = clie_fd;//将端口号放进对应的用户信息中
                                        send_data.option = LOGIN_USERPASSWORD;
                                        strcpy(send_data.mesg_data,"输入密码：");
                                        send_data.chatroom_authority = temp->authority;
                                        write(clie_fd,&send_data,sizeof(struct SerToCliFrame));
                                }//处理存在和发送确认信息

                        }
                        return 0;
                case LOGIN_USERPASSWORD:
                        {
                                PtrUserDate temp;
                                if((temp = SearchUser(g_user_list,FdToUsername(clie_fd))) == NULL )
                                {
                                        Log("密码验证错误，端口号找不到相映射的数据","server");
                                        send_data.option =  ERROR_TEXT;
                                        strcpy(send_data.mesg_data,"服务器处理错误");
                                        write(clie_fd,&send_data,sizeof(struct SerToCliFrame));
                                }else{
                                       if( strcmp(temp->password,get_data.mesg_data) == 0 )
                                       {
                                               temp->status = USER_STATUS_UP;//上线
                                               send_data.chatroom_authority = temp->authority;
                                               send_data.option = LOGIN_SUCCESE;
                                               Log("登陆成功",temp->name);
                                               write(clie_fd,&send_data,sizeof(struct SerToCliFrame));
                                       }else{
                                               send_data.option = ERROR_USERPASS_WRONG;
                                               write(clie_fd,&send_data,sizeof(struct SerToCliFrame));
                                       }
                                }//if((temp = SearchUser(g_user_list,FdToUsername(clie_fd))) == NULL ) 判断和处理密码
                        }
                        return 0;
                case REQUEST_EXIT:
                        return 1;
                default:
                        ProcessMesg(clie_fd,&get_data);
                        return 0;
        }

}

void ProcessMesg(int clie_fd,struct CliToSerFrame * get_data)
{
       PtrUserDate head;

       for( head=g_user_list->next; head!=NULL; head=head->next)
       {
               MySend(clie_fd,head->confd,get_data->mesg_data);
       }

}

int  MySend( int source_fd,int target_fd,char * send_string )
{
        PtrUserDate temp;
        struct SerToCliFrame  send_data;
        temp = SearchUser(g_user_list,FdToUsername(target_fd)); /*  */
        send_data.chatroom_authority = temp->authority; /* */
        strcpy(send_data.mesg_data,send_string); /*  */
        send_data.option = SEND_MESG;           /*  */
        send_data.send_time = time(NULL);       /*  */
        strcpy(send_data.sender_name,FdToUsername(source_fd)); /*  */
        if( write(target_fd,&send_data,sizeof(struct SerToCliFrame)) < 0 )
        {
                Log("send mesg eror",FdToUsername(source_fd));
                return -1;
        }
        return 0;
        

}
