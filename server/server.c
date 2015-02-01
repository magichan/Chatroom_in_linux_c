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
                flag = select(Maxfd+1,&readfile,NULL,NULL,NULL);
                if( flag >= 1 )
                {
                        int i;
                        if( FD_ISSET(g_sock_fd,&readfile) )
                        {
                               struct sockaddr_in  clie_addr;//请求连接者地址
                               socklen_t serv_addr_len;
                               serv_addr_len = sizeof(struct sockaddr_in);
                              clie_fd = accept(g_sock_fd,(struct sockaddr *)&clie_addr,&serv_addr_len);
                              if(clie_fd < 0 )         Log("连接失败","client");
                              else{
                                    printf("Ip : %s 请求连接\n",inet_ntoa(clie_addr.sin_addr));
                                   for( i=0; i<CLIENT_MAX; i++ )
                                   {
                                           if( AllClieFd[i] < 0 )
                                           {
                                                   AllClieFd[i] = clie_fd;
                                                   break;
                                           }
                                   }//for( i=0; i<CLIENT_MAX;i++),为clie_fd找个新的位置 

                                   if( i >= CLIENT_MAX ) 
                                   {
                                           memset(&send_data,0,sizeof(struct SerToCliFrame)); 
                                           send_data.option = ERROR_EXCEEDING;
                                           Log("登陆过多，拒绝访问\n",inet_ntoa(clie_addr.sin_addr));
                                           write(clie_fd,&send_data,sizeof(struct SerToCliFrame));
                                           close(clie_fd);
                                   }else{
                                           FD_ISSET(clie_fd,&Allreadfile);
                                           Maxfd = clie_fd>Maxfd?clie_fd:Maxfd;
                                           Log("请求连接",inet_ntoa(clie_addr.sin_addr));
                                   }//if( i >= CLIENT_MAX ) ，处理超额情况
                              }//if(clie_fd<0)

                        }// if( FD_ISSET(g_sock_fd,&readfile) ) 

                        for( i=0; i< CLIENT_MAX; i++ )
                        {
                                if(FD_ISSET(AllClieFd[i],&readfile))
                                {
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

        
        if((count = read(clie_fd,&get_data,sizeof(struct CliToSerFrame))) == 0 )
        {
                return -1;
        }
        memset(&send_data,0,sizeof(struct SerToCliFrame));

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
                                if( SearchUser(g_user_list,get_data.mesg_data) != NULL )
                                {
                                        PtrUserDate temp = (struct UserDate *)malloc(sizeof(struct UserDate));
                                        strcpy(temp->name,get_data.mesg_data);
                                       


                                }

                        }
                        return 0;
                case REGISTER_USERPASSWORD:
                        {

                        }
                        return 0;
                case REQUEST_LOGIN:
                        {

                        }
                        return 0;
                case LOGIN_USERNAME:
                        {

                        }
                        return 0;
                case LOGIN_USERPASSWORD:
                        {

                        }
                        return 0;
                case REQUEST_EXIT:
                        return 1;
                default:
                        ProcessMesg(clie_fd,&get_data);
                        return 0;
        }

}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  LogOrRegist
 *  Description:  登陆或是注册
 *        Entry:  服务端端口号,服务端地址
 *         Exit:
 * =====================================================================================
 */
void LoginOrRegist( int clie_fd ,struct sockaddr_in * clie_addr)
{
        int flag;
        struct CliToSerFrame data;    // 存放发送过来的数据帧
        struct SerToCliFrame send_data;
        
        flag = read(clie_fd,&data,sizeof(struct CliToSerFrame));
        if( flag <= 0)
        {
                MyErrorPthread("read\n",__FUNCTION__,__LINE__);
        }

        switch( data.option )//判断数据包
        {
                case REQUIE_REGISTER :
                        Regist(clie_fd,clie_addr);
                        break;
                case REQUEST_LOGIN :
                        Login(clie_fd,clie_addr);
                        break;
                default:
                     memset(&send_data,0,sizeof(struct SerToCliFrame)); 
                     send_data.option = ERROR_PROCESS;
                     Log("登陆流程错误\n",inet_ntoa(clie_addr->sin_addr));
                     write(clie_fd,&send_data,sizeof(struct SerToCliFrame));
                     pthread_exit((void *)1);
        }
}

void Login( int clie_fd,struct sockaddr_in * clie_addr)
{
        struct SerToCliFrame send_data;
        memset(&send_data,0,sizeof(struct SerToCliFrame));
        send_data.option =  LOGIN_USERNAME;
        strcpy(send_data.mesg_data,"请输入用户名:");
        write(clie_fd,&send_data,sizeof(struct SerToCliFrame));



}
void Regist(int clie_fd,struct sockaddr_in * clie_addr)
{

}


