/*
 * =====================================================================================
 *
 *       Filename:  client.h
 *
 *    Description
 *
 *        Version:  1.0
 *        Created:  2015年02月03日 21时12分22秒
 *       Revision:  none
 *       Compiler:  clang 
 *
 *         Author:  magichan (fgm), 574932286@qq.com
 *   Organization:  FH Südwestfalen, Iserlohn
 *
 * =====================================================================================
 */
#ifndef _CLIENT_H_
#define _CLIENT_H_
#include"tool.h"
#include"frame.h"
#include"debug.h"
#include"interface.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>    // 系统调用的封装
#include<pthread.h>   //线程
#include<sys/socket.h>//套接字编程
#include<sys/types.h> //定义基本数据类型
#include<netinet/in.h>
#include<arpa/inet.h> //字节顺序和网络地址的转换



/*                  错误编码            */
#define         ERROR_PROCESS                           -1 /* 数据包发送流程错误 */
#define         ERROR_USERNAME_EXISTENCE                -2 /*  用户存在*/
#define         ERROR_USRENAME_NOEXISTENCE              -3 /*  用户名不存*/
#define         ERROR_USERPASS_WRONG                    -4 /*  密码错误*/
#define         ERROR_EXCEEDING                         -5 /*  客户端过多*/
#define         ERROR_TEXT                              -6 /*  错误描述见数据*/
#define         ERROR_USER_HAVE_UP                      -7 /*  该用户已经登陆*/
#define         ERROR_COMD_NOEXISTENCE                  -8 /*  命令无法解析*/

int  Login(int serv_fd);
int  Regist(int serv_fd);
void   Chat(int serv_fd);
void GetMesg( void *  serv );
void DealMesg( struct SerToCliFrame  * get_data ); 

#endif
