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
pthread_mutex_t g_mutex; //锁
int AllClieFd[CLIENT_MAX] = {-1};//存放所有的套接字
fd_set Allreadfile;//select所用 的文件描述符


int main( void )
{
        int clie_fd;
        pthread_t exit_pthread;
        int Maxfd;//存放最大的文件描述符
        struct SerToCliFrame send_data;
        fd_set readfile,Allreadfile;//select所用 的文件描述符
        pthread_t pthread_slove_connect;
        
        /*用户列表初始化*/
       g_user_list =  InitUserList();
       /*套接字初始化*/
       InitSock();
       /* 互斥锁的初始化*/
       pthread_mutex_init(&g_mutex,NULL);
       /* 创建一个新的线程负责退出机制*/
       pthread_create(&exit_pthread,NULL,(void * )Myexit,NULL);
        

        FD_ZERO(&readfile);
        FD_SET(g_sock_fd,&Allreadfile);
        Maxfd = g_sock_fd;
/*         
 *         遗留问题，主线程无法退出
 *         开辟另一线程来处理结束命令
 */
        while(1)//循环等待
        {
                readfile = Allreadfile;
                int flag;

                flag = select(Maxfd+1,&readfile,NULL,NULL,NULL);

                if( flag <= 0 )
                {
                        MyError("select\n",__FUNCTION__,__LINE__);
                }
                        
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
                                           if( AllClieFd[i] <= 0 )
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
                                           Mywrite(clie_fd,&send_data,sizeof(struct SerToCliFrame));
                                           close(clie_fd);
                                   }else{
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
                                        //input_msg("%d 的文件描述符有信息发送过来\n",AllClieFd[i]);
                                        pthread_mutex_lock(&g_mutex);
                                       switch(AnalyzeMesg( AllClieFd[i] ))
                                       {
                                               case 1:
                                                       Log("申请退出",FdToUsername(AllClieFd[i]));
                                                       close(AllClieFd[i]);
                                                       FD_CLR(AllClieFd[i],&Allreadfile);
                                                       AllClieFd[i] = -1;
                                                       break;
                                               case -1:Log("非法退出","麻烦");
                                                       close(AllClieFd[i]);
                                                       FD_CLR(AllClieFd[i],&Allreadfile);
                                                       AllClieFd[i] = -1;
                                                       break;
                                               default:
                                                       break;
                                       }//switch 处理客户端关闭，并且重置空间
                                       pthread_mutex_unlock(&g_mutex);
                                }// if(FD_ISSET(AllClieFd[i],&readfile))
                        }//for( i=0; i< CLIENT_MAX; i++ ),处理有数据发送过来的套接字
                }//if(flag > = 1 )
        }//while(1)

}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Myexit
 *  Description:  获取服务器输入退出，线程的异步性
 *        Entry:  
 *         Exit:
 * =====================================================================================
 */
void Myexit( void )
{
        char commnd[USER_MAX];
        while(1)
        {
                GetInfo(commnd,USER_MAX);
                if( strcmp(commnd,"exit") == 0 )
                {
                        pthread_mutex_lock(&g_mutex);
                        
                                PtrUserDate p;

                                /* 发送信息处理 */
                                struct SerToCliFrame send_data;
                                memset(&send_data,0,sizeof(struct SerToCliFrame));
                                send_data.option = REQUEST_EXIT;
                                strcpy(send_data.mesg_data,"服务器关闭");

                                p = g_user_list->next;
                                while( p!=NULL )
                                {
                                        write(p->confd,&send_data,sizeof(struct SerToCliFrame));
                                        p = p->next;
                                }
                                WriteUserList(g_user_list);
                        pthread_mutex_unlock(&g_mutex);
                        pthread_mutex_destroy(&g_mutex);
                        printf("++++++++++++++++++++CLOSE++++++++++++++++++++++++\n");
                        sleep(1);
                        exit(0);
                        
                }//判断是否是退出请求
        }//等待命令
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
//        input_msg("接收到的数据 : option :%d, mesg: %s\n",get_data.option,get_data.mesg_data);
        memset(&send_data,0,sizeof(struct SerToCliFrame));
/* 
 *    管理员和普通用户的注册问题：一旦管理员用户注册失败，同时还排列这普通用户，就会出问题
 */


  //      pthread_mutex_lock(&g_mutex);
        switch( get_data.option)
        {
                case REQUIE_REGISTER:
                        {
                                if( UserListIsEmpty(g_user_list) )
                                {
                                          send_data.option = REGISTER_USERNAME;
                                          strcpy(send_data.mesg_data,"输入用户名[管理员]：");
                                          Mywrite(clie_fd,&send_data,sizeof(struct SerToCliFrame));

                                }else{
                                          send_data.option = REGISTER_USERNAME;
                                          strcpy(send_data.mesg_data,"输入用户名：");
                                          Mywrite(clie_fd,&send_data,sizeof(struct SerToCliFrame));
                                }//if，判断是否建立root 账号
                        }
                        return 0;
                case REGISTER_USERNAME:
                        {
                                if( SearchUser(g_user_list,get_data.mesg_data) == NULL )
                                {
                                        input_msg("姓名请求获得");
                                        PtrUserDate temp = (struct UserDate *)malloc(sizeof(struct UserDate));
                                        strcpy(temp->name,get_data.mesg_data);
                                        temp->status = USER_STATUS_DEALING;//待处理的用户结构
                                        temp->confd = clie_fd;
                                        AddUser(g_user_list,temp);//先将待处理的数据结构添加
                                        send_data.option = REGISTER_USERPASSWORD;

                                        strcpy(send_data.mesg_data,"输入密码:");
                                        Mywrite(clie_fd,&send_data,sizeof(struct SerToCliFrame));//将密码要求发送
                                }else{
                                        send_data.option = ERROR_USERNAME_EXISTENCE;
                                        Log("申请的姓名重复，拒绝注册","麻烦");
                                        Mywrite(clie_fd,&send_data,sizeof(struct SerToCliFrame));
                                }//用户名重复

                        }
                        return 0;
                case REGISTER_USERPASSWORD:
                        {
                                PtrUserDate temp;

                                if((temp=SearchUser(g_user_list,FdToUsername(clie_fd))) == NULL )
                                {
                                        Log("密码验证错误，端口号找不到相映射的数据","server");
                                        send_data.option =  ERROR_TEXT;
                                        strcpy(send_data.mesg_data,"服务器处理错误");
                                        Mywrite(clie_fd,&send_data,sizeof(struct SerToCliFrame));
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

                                        temp->status = USER_STATUS_DOWN;//下线

                                        send_data.option = REGISTER_SUCCSE;
                                        Mywrite(clie_fd,&send_data,sizeof(struct SerToCliFrame));
                                        Log("注册成功",temp->name);
                                }//if((temp = SearchUser(g_user_list,FdToUsername(clie_fd))) == NULL ) 判断和处理密码

                        }
                        return 0;
                case REQUEST_LOGIN:
                        {
                                send_data.option = LOGIN_USERNAME;
                                strcpy(send_data.mesg_data,"输入您的用户名:");
                                Mywrite(clie_fd,&send_data,sizeof(struct SerToCliFrame));
                        }
                        return 0;
                case LOGIN_USERNAME:
                        {
                                PtrUserDate temp;
                                if( (temp=SearchUser(g_user_list,get_data.mesg_data)) == NULL )
                                {
                                        send_data.option = ERROR_USRENAME_NOEXISTENCE;
                                        Mywrite(clie_fd,&send_data,sizeof(struct SerToCliFrame));
                                }else{

                                        if( temp->status == USER_STATUS_DOWN )
                                        {
                                                temp->confd = clie_fd;//将端口号放进对应的用户信息中
                                                send_data.option = LOGIN_USERPASSWORD;
                                                strcpy(send_data.mesg_data,"输入密码：");
                                                send_data.chatroom_authority = temp->authority;
                                                Mywrite(clie_fd,&send_data,sizeof(struct SerToCliFrame));
                                        }else{
                                                send_data.option = ERROR_USER_HAVE_UP;
                                                strcpy(send_data.mesg_data,"该用户已经登陆");
                                                Mywrite(clie_fd,&send_data,sizeof(struct SerToCliFrame));
                                        }//处理是否重复登陆
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
                                        Mywrite(clie_fd,&send_data,sizeof(struct SerToCliFrame));
                                }else{
                                       if( strcmp(temp->password,get_data.mesg_data) == 0 )
                                       {
                                               temp->status = USER_STATUS_UP;//上线
                                               send_data.chatroom_authority = temp->authority;
                                               send_data.option = LOGIN_SUCCESE;
                                               Log("登陆成功",temp->name);
                                               Mywrite(clie_fd,&send_data,sizeof(struct SerToCliFrame));
                                       }else{
                                               send_data.option = ERROR_USERPASS_WRONG;
                                               strcpy(send_data.mesg_data,"密码错误，请重新登陆\n");
                                               Mywrite(clie_fd,&send_data,sizeof(struct SerToCliFrame));
                                       }
                                }//if((temp = SearchUser(g_user_list,FdToUsername(clie_fd))) == NULL ) 判断和处理密码
                        }
                        return 0;
                case REQUEST_EXIT:
                          {

                                   PtrUserDate cli;
                                  int ki;
                                  cli = SearchUser(g_user_list,FdToUsername(clie_fd));
                                  if( cli == NULL )
                                              return 1;
                                  else{
                                           cli->status = USER_STATUS_UP;
                                           return 1;
                                  }//处理登陆流程中和登陆后客户端的请求退出
                          }
                default:
                        ProcessMesg(clie_fd,&get_data);
                        return 0;
        }//swtich
//        pthread_mutex_unlock(&g_mutex);

}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ProcessMesg
 *  Description:  处理非登陆和注册流程的 命令 请求
 *        Entry:   
 *         Exit:
 * =====================================================================================
 */
void ProcessMesg(int clie_fd,struct CliToSerFrame * get_data)
{
       PtrUserDate head;
       PtrUserDate temp;
       int counnt;
       int target_fd;
       char commnd_conist[15][USER_MAX];
       struct SerToCliFrame send_data;
       temp=SearchUser(g_user_list,FdToUsername(clie_fd));

       switch( get_data->option)
       {
               case SEND_MESG:
                          if( temp->authority == CLIENT_STATUS_AUDIENCE )
                          {
                                  SendError(clie_fd,ERROR_LACK_OF_PERMISSION,"权限不足");
                                  break;
                          }
                          for( head=g_user_list->next; head!=NULL; head=head->next)
                          {
                                MySend(clie_fd,head->confd,get_data->mesg_data,SEND_MESG);
                          }
                          break;
               case SEND_COMD:

                          counnt = DealCommond(get_data->mesg_data,commnd_conist);
#if(1)
                         {
                                 int i;
                                 for(i=0; i<counnt; i++ )
                                 {
                                         printf("%s\n",commnd_conist[i]);
                                 }
                         }
#endif
                          if(strcmp(commnd_conist[0],"set")==0 )
                          {
                                  if(DealComdSet(clie_fd,commnd_conist,counnt) < 0 )
                                  {
                                          Log("处理set命令发生错误",FdToUsername(clie_fd));
                                          input_msg("处理set命令发生错误   %s ",FdToUsername(clie_fd));
                                  }
                          }else if( strcmp(commnd_conist[0],"list")==0 || strcmp(commnd_conist[0],"l")==0 )
                          {
 //                                 input_msg("成功获取 list 指令 \n");
                                  if(DealComdList(clie_fd,commnd_conist,counnt)<0)
                                  {
                                          Log("处理list命令发生错误",FdToUsername(clie_fd));
                                          input_msg("处理list命令发生错误   %s ",FdToUsername(clie_fd));

                                  }
                          }else{
                                  SendError(clie_fd,ERROR_TEXT,"处理命令发生错误");
                          }

                          break;
               case SEND_PRIVATE_MESG:
                          temp = SearchUser(g_user_list,get_data->target_name);
                          if( temp == NULL )
                          {
                                SendError(clie_fd,ERROR_USRENAME_NOEXISTENCE,"该用户不存在");
                                break;
                          }
                          if( temp->authority == USER_STATUS_DOWN )
                          {
                                  SendError(clie_fd,ERROR_USER_HAVE_DOWN,"该用户不在线");
                                        break;
                          }
                          MySend(clie_fd,temp->confd,get_data->mesg_data,SEND_PRIVATE_MESG);
                        
                          break;
               case REQUEST_EXIT:
                          {
                                  PtrUserDate cli;
                                  int ki;
                                  cli = SearchUser(g_user_list,FdToUsername(clie_fd));
                                  cli->status = USER_STATUS_UP;
                                  for( ki=0; ki<CLIENT_MAX; ki++ )
                                  {
                                          if( AllClieFd[ki] == clie_fd )
                                          {
                                               Log("申请退出",FdToUsername(clie_fd));
                                               close(clie_fd);
                                               FD_CLR(clie_fd,&Allreadfile);
                                               AllClieFd[ki] = -1;
                                               break;
                                          }
                                  }
                          }

               default:
                          /*处理不存在指令*/
                          memset(&send_data,0,sizeof(struct SerToCliFrame));
                          send_data.option = ERROR_COMD_NOEXISTENCE;
                          strcpy(send_data.mesg_data,"该操作不存在");
                          write(clie_fd,&send_data,sizeof(struct SerToCliFrame));
       }

}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  MySend
 *  Description:  向 客户端 发送信息，
 *        Entry:  发送者 ： source_fd , 接受者 target_fd , 发送信息 send_string , 群聊/私聊选项 chose 
 *         Exit:  成功返回 0 失败返回 -1
 * =====================================================================================
 */

int  MySend( int source_fd,int target_fd,char * send_string ,int chose )
{
        PtrUserDate temp;
        struct SerToCliFrame  send_data;
        temp = SearchUser(g_user_list,FdToUsername(target_fd)); /*  */
        send_data.chatroom_authority = temp->authority; /* */
        strcpy(send_data.mesg_data,send_string); /*  */
        send_data.option = chose;           /*  */
        send_data.send_time = time(NULL);       /*  */
        strcpy(send_data.sender_name,FdToUsername(source_fd)); /*  */
        if( write(target_fd,&send_data,sizeof(struct SerToCliFrame)) < 0 )
        {
                Log("send mesg eror",FdToUsername(source_fd));
                return -1;
        }
        return 0;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  DealComdSet
 *  Description:  处理命令 set 
 *        Entry: 发送者的套接字 commnd的解析部分
 *         Exit: 成功返回 0 ，失败返回 -1【找不到clie_fd】 , -2 【 参数错误 】
 * =====================================================================================
 */
int DealComdSet(int clie_fd, char (*commnd_conist)[USER_MAX],int counnt)
{
            PtrUserDate send_temp,temp;
            send_temp  = SearchUser(g_user_list,FdToUsername(clie_fd));
            
            if( send_temp->authority != CLIENT_STATUS_ROOT )
            {
                    SendError(clie_fd,ERROR_LACK_OF_PERMISSION,"权限不足");
                    input_msg("%s 权限不足 \n",send_temp->name);
                    return -1;
            }
            if((strcmp(commnd_conist[1],"-c")!=0&&strcmp(commnd_conist[1],"-l")!=0&&strcmp(commnd_conist[1],"-k")!=0)
                            || counnt <=2 )
            {
                    SendError(clie_fd,ERROR_PARAMETER,"set 缺乏必要的参数,参照:help");
                    input_msg("set 缺乏必要的参数\n");
                    return -2;
            }
            
            if(1)//将三个命令总结为一个
            {
                    int j=2;
                    int flag;

                    for( ;j<counnt; j++ )
                    {
                            temp = SearchUser(g_user_list,commnd_conist[j]);
                            if( temp == NULL )
                            { 
                                    int char_count=0;
                                    char temp_string[MESG_MAX];
                                    while(commnd_conist[j][char_count] != '\0' )
                                    {
                                            temp_string[char_count] = commnd_conist[j][char_count];
                                            char_count++;
                                    }
                                    strcpy(&temp_string[char_count]," 未找到该用户");
                                    SendError(clie_fd,ERROR_TEXT,temp_string);
                                    continue;
                            }//if( temp == NULL ) 处理 未找到该用户的问题，并解决 汇报错误时，字符串的连接问题

                            if( strcmp(commnd_conist[1],"-l")== 0 )
                            {
                                  temp->authority = CLIENT_STATUS_AUDIENCE;//设置为旁听模式
                                  flag = SendRequest(temp->confd,CHANGE_AUTHORITY,NULL); 
                            }
                            if( strcmp(commnd_conist[1],"-c")== 0 )
                            {
                                  temp->authority = CLIENT_STATUS_COMMON;//设置为旁听模式
                                  flag = SendRequest(temp->confd,CHANGE_AUTHORITY,NULL); 
                            }
                            if( strcmp(commnd_conist[1],"-k")== 0 )
                            {
                                  flag = SendRequest(temp->confd,REQUEST_EXIT,NULL); 
                            }

                            if( flag < 0 )
                                {
                                    int char_count=0;
                                    char temp_string[MESG_MAX];
                                    while(commnd_conist[j][char_count] != '\0' )
                                    {
                                            temp_string[char_count] = commnd_conist[j][char_count];
                                            char_count++;
                                    }
                                    strcpy(&temp_string[char_count]," 未发送成功");
                                    SendError(clie_fd,ERROR_TEXT,temp_string);
                            }//if( (.....)<0) 解决发送失败的汇报问题

                    }//发送给要求的所有客户端
            }//设置
            return 0;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  DealComdList
 *  Description:  处理list 命令
 *        Entry:  
 *         Exit:  成功返回 0
 *                参数错误 返回 -1
 *                write 错误 返回 -2
 * =====================================================================================
 */
int DealComdList(int clie_fd, char (*commnd_conist)[USER_MAX],int counnt)
{
        PtrUserDate temp;
        int i=0;
        struct SerToCliFrame send_data;
        int up_usr_count=0;
        char temp_string[MESG_MAX];
        temp = g_user_list->next;
        if( counnt > 1 )
        {
                SendError(clie_fd,ERROR_PARAMETER,"参数错误，请查询:help");
                return -1;
        }
        memset(&send_data,0,sizeof(struct  SerToCliFrame));

        while( temp!=NULL )
        {
                if( temp->status == USER_STATUS_UP )
                {
                        up_usr_count++;
                }
                temp = temp->next;
        }
        sprintf(temp_string,"上线人数:%d\n",up_usr_count);

        temp = g_user_list->next;
        while( temp!=NULL )
        {
                if( temp->status == USER_STATUS_UP )
                {
                        strcat(temp_string,temp->name);
                        strcat(temp_string,"  ");
                        i++;
                        if( i%3 == 0 )
                        {
                                strcat(temp_string,"\n");
                        }
                }
                temp = temp->next;
        }
        strcat(temp_string,"\n*********end********\n");
        send_data.option = USER_LIST_MESG;
        strcpy(send_data.mesg_data,temp_string);
        if( write(clie_fd,&send_data,sizeof(struct SerToCliFrame)) < 0 )
        {
                MyError("write",__FUNCTION__,__LINE__);
                return -2;
        }
        return 0;

}
