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
        int optval;
        fd_set readfile;//select所用 的文件描述符
        pthread_t pthread_slove_connect;
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

        FD_ZERO(&readfile);
        FD_SET(g_sock_fd,&readfile);
/*         
 *         遗留问题，主线程无法退出
 *         可能的处理方法：信号
 */
        while(1)//循环等待
        {
                int flag;
                flag = select(1,&readfile,NULL,NULL,NULL);
                if( flag == 1 )//有数据到达套接字，处理链接
                {
                        if(pthread_create(&pthread_slove_connect,NULL,(void *)SloveConnect,NULL) != 0 )
                        {
                                MyError("pthread_create",__FUNCTION__,__LINE__);
                        }
                }
        }



}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  SloveConnect
 *  Description:  处理服务端连接
 *        Entry:  空
 *         Exit:  空
 * =====================================================================================
 */
void SloveConnect(void)
{
        int clie_fd;
        int flag;
        struct sockaddr_in  clie_addr;//请求连接者地址
        socklen_t serv_addr_len;
        struct CliToSerFrame data;    // 存放发送过来的数据帧
        serv_addr_len = sizeof(struct sockaddr_in);

        /*接受连接*/
        clie_fd = accept(g_sock_fd,(struct sockaddr *)&clie_addr,&serv_addr_len);
        if(clie_fd < 0 )
        {
                MyErrorPthread("accept",__FUNCTION__,__LINE__);
        }

        /*日志和输出*/
        Log("请求连接\n",inet_ntoa(clie_addr.sin_addr));
        printf("Ip : %s 请求连接\n",inet_ntoa(clie_addr.sin_addr));
       

        flag = read(clie_fd,&data,sizeof(struct CliToSerFrame));
}

void LogOrRegist( int clie_fd,const struct CliToSerFrame * data )
{

}



