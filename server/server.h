/*
 * =====================================================================================
 *
 *       Filename:  client.h
 *
 *    Description:  服务器基本函数定义和命令宏定义
 *
 *        Version:  1.0
 *        Created:  2015年01月28日 21时15分18秒
 *       Revision:  none
 *       Compiler:  clang 
 *
 *         Author:  magichan (fgm), 574932286@qq.com
 *   Organization:  FH Südwestfalen, Iserlohn
 *操
 * =====================================================================================
 */

#ifndef _CLIENT_H_
#define _CLIENT_H_

#include"frame.h"
#include"user.h"
#include"debug.h"
#include"tool.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>    // 系统调用的封装
#include<pthread.h>   //线程
#include<sys/socket.h>//套接字编程
#include<sys/types.h> //定义基本数据类型
#include<sys/select.h>
#include<netinet/in.h>
#include<arpa/inet.h> //字节顺序和网络地址的转换

/*          服务器基本参数         */

#define         SERV_PORT       4507      /*服务器端口号*/
#define         LISTENQ         12        /*连接请求的最大队列长度*/
#define         CLIENT_MAX      16        /*服务端最大个数*/
/*           服务器向客户端发送的命令  */

#define         ERROR_PROCESS                           -1 /* 数据包发送流程错误 */
#define         ERROR_USERNAME_EXISTENCE                -2 /*  用户存在*/
#define         ERROR_USRENAME_NOEXISTENCE              -3 /*  用户名不存*/
#define         ERROR_USERPASS_WRONG                    -4 /*  密码错误*/
#define         ERROR_EXCEEDING                         -5 /*  客户端过多*/
#define         ERROR_TEXT                              -6 /*  错误描述见数据*/
#define         ERROR_USER_HAVE_UP                      -7 /*  该用户已经登陆*/
#define         ERROR_COMD_NOEXISTENCE                  -8 /*  命令无法解析*/
#define         ERROR_LACK_OF_PERMISSION                -9 /*  权限不足 */
#define         ERROR_PARAMETER                         -10 /*参数错误*/
#define         ERROR_USER_HAVE_DOWN                    -11 /* 该用户已下线*/
void  InitSock(void);
int   AnalyzeMesg( int clie_fd );
void ProcessMesg(int clie_fd,struct CliToSerFrame * get_data);
int  MySend( int source_fd,int target_fd,char * send_string ,int chose );
void Myexit( void );




#endif

