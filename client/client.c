/*
 * =====================================================================================
 *
 *       Filename:  client.c
 *
 *    Description:  客户端
 *
 *        Version:  1.0
 *        Created:  2015年02月03日 21时10分47秒
 *       Revision:  none
 *       Compiler:  clang 
 *
 *         Author:  magichan (fgm), 574932286@qq.com
 *   Organization:  FH Südwestfalen, Iserlohn
 *           Rule:  ???? question 
 *                  [    comment
 *
 * =====================================================================================
 */
#include"client.h"
int  authority = 0; //客户端权限标志符

int main( int argc,char * argv[] )
{
        
        struct sockaddr_in serv_addr;
        int i;
        struct CliToSerFrame send_data;
        int choice;
        int serv_port;
        int conn_fd;
        if( argc !=5 )
        {
                printf("Usage : [-p] [serv_port] [-a] [serv_addres]\n");
                exit(1);
        }

        memset(&serv_addr,0,sizeof(struct sockaddr_in));
        serv_addr.sin_family = AF_INET;
        for( i=1; i<argc; i++)
        {
                if( strcmp("-p",argv[i])==0 )
                {
                        serv_port = atoi(argv[i+1]);
                        serv_addr.sin_port = htons(serv_port);
                        continue;
                }
                if( strcmp("-a",argv[i])==0 )
                {
                        inet_aton( argv[i+1],&serv_addr.sin_addr);
                        continue;
                }
        }

        if( serv_addr.sin_port == 0 || serv_addr.sin_addr.s_addr==0 )
        {
                printf("Usage : [-p] [serv_port] [-a] [serv_addres]\n");
                exit(1);
        }

        conn_fd  = socket(AF_INET,SOCK_STREAM,0);
        if( connect(conn_fd,(struct sockaddr *)&serv_addr,sizeof(struct sockaddr))< 0 )
        {
                MyError("connect",__FUNCTION__,__LINE__);
        }


        InterfaceWelcome();
        choice = getchar();
        while(choice != '3')
        {
                switch( choice )
                {
                        case '1':  if( Login(conn_fd) )
                                   {
                                         InterfaceWelcome();
                                   }else{
                                           /*解决正确登录及主函数退出问题*/
                                           //exit(0);
                                   }//判断是否登录成功
                                break;
                        case '2':
                                if( Regist(conn_fd) )
                                {
                                        InterfaceWelcome();
                                }
                                break;
                        case '3':
                                 memset(&send_data,0,sizeof(struct CliToSerFrame));
                                 send_data.option = REQUEST_EXIT;
                                 write(conn_fd,&send_data,sizeof(struct CliToSerFrame));
                                 sleep(2);
                                 InterfaceBye();
                                 exit(0); 
                                /*退出操作*/
                        break;
                        defalut:
                                break;
                }// switch(choiec) 分析命令
                choice = getchar();
        }//获取命令

}

int  Login( int serv_fd)
{

        return 0;
}
int  Regist( int serv_fd )
{
        struct CliToSerFrame send_data;
        struct SerToCliFrame get_data;

        /*发送申请*/
        memset(&send_data,0,sizeof(struct CliToSerFrame));
        send_data.option = REQUIE_REGISTER;
        if( write(serv_fd,&send_data,sizeof(struct CliToSerFrame))<0)
        {
                MyError("write",__FUNCTION__,__LINE__);
        }
        memset(&get_data,0,sizeof(struct SerToCliFrame));
        if(read(serv_fd,&get_data,sizeof(struct SerToCliFrame))<=0)
        {
                  MyError("read",__FUNCTION__,__LINE__);
        }
        input_msg("  从套接字读取的数据为option:%d,mesg:%s\n",get_data.option,get_data.mesg_data);

        while(1)
        {
                switch(get_data.option)
                {
                        case REGISTER_USERNAME:
                                {
                                         int flag;
                                         memset(&send_data,0,sizeof(struct CliToSerFrame));
                                         send_data.option = REGISTER_USERNAME;
                                         printf("%s",get_data.mesg_data);
                                       while((flag=GetInfo(send_data.mesg_data,USER_MAX)) != 0 )
                                       {
                                               if(flag == -1 )
                                                {
                                                        MyError("get_info",__FUNCTION__,__LINE__);
                                                }
                                               if(flag == -2 )
                                               {
                                                       Myfflush();
                                                       printf("用户名过长,请重新输入:");
                                                       continue;
                                               }
                                       }//获取用户名
                                       write(serv_fd,&send_data,sizeof(struct CliToSerFrame));
                                break;
                                }
                        case REGISTER_USERPASSWORD:
                                {
                                         int flag;
                                         memset(&send_data,0,sizeof(struct CliToSerFrame));
                                         send_data.option = REGISTER_USERPASSWORD;
                                         printf("%s",get_data.mesg_data);
                                       while((flag=GetInfo(send_data.mesg_data,USER_MAX)) != 0 )
                                       {
                                               if(flag == -1 )
                                                {
                                                        MyError("get_info",__FUNCTION__,__LINE__);
                                                }
                                               if(flag == -2 )
                                               {
                                                       Myfflush();
                                                       printf("密码过长,请重新输入:");
                                                       continue;
                                               }
                                       }//获取密码
                                       write(serv_fd,&send_data,sizeof(struct CliToSerFrame));
                                }
                                break;
                        case REGISTER_SUCCSE:
                                {
                                        InterfaceRegistSucces();
                                        memset(&send_data,0,sizeof(struct CliToSerFrame));
                                        send_data.option = REQUEST_EXIT;
                                        write(serv_fd,&send_data,sizeof(struct CliToSerFrame));
                                        InterfaceBye();
                                        exit(1);
                                }
                                break;
                        case ERROR_USERNAME_EXISTENCE:
                                {
                                         int flag;
                                         memset(&send_data,0,sizeof(struct CliToSerFrame));
                                         send_data.option = REGISTER_USERNAME;
                                        Myfflush();
                                        printf("重新输入用户名:");
                                       while((flag=GetInfo(send_data.mesg_data,USER_MAX)) != 0 )
                                       {
                                               if(flag == -1 )
                                                {
                                                        MyError("get_info",__FUNCTION__,__LINE__);
                                                }
                                               if(flag == -2 )
                                               {
                                                       Myfflush();
                                                       printf("用户名过长,请重新输入:");
                                                       continue;
                                               }
                                       }//获取密码
                                       write(serv_fd,&send_data,sizeof(struct CliToSerFrame));
                                }
                                break;
                        case ERROR_TEXT:
                                printf("%s\n",get_data.mesg_data);
                                sleep(2);
                                exit(0);
                                break;
                        case ERROR_EXCEEDING:
                                printf("%s\n",get_data.mesg_data);
                                sleep(1);
                                exit(2);
                        default:
                                input_msg("无法解释的 option ：%d,和 mesg:%s\n",get_data.option,get_data.mesg_data);
                                MyError("server 返回错误",__FUNCTION__,__LINE__);

                }
                memset(&get_data,0,sizeof(struct SerToCliFrame));
               if(read(serv_fd,&get_data,sizeof(struct SerToCliFrame))<=0)
               {
                       MyError("read",__FUNCTION__,__LINE__);
               }
                input_msg("  从套接字读取的数据为option:%d,mesg:%s\n",get_data.option,get_data.mesg_data);
        }

}
