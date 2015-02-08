/*
 * =====================================================================================
 *
 *       Filename:  tool.h
 *
 Description:  提供一些工具和日志函数
 *
 *        Version:  1.0
 *        Created:  2015年01月30日 16时37分09秒
 *       Revision:  none
 *       Compiler:  clang 
 *
 *         Author:  magichan (fgm), 574932286@qq.com
 *   Organization:  FH Südwestfalen, Iserlohn
 *
 * =====================================================================================
 */
#include"server.h"
#include"user.h"
 /* 日志的宏定义 */
#define   LOG_SERVER    -1
void MyError(char * err_string,const char * function, int line);
void MyErrorPthread(char * err_string,const char * function, int line);
void Log(const char *log_string, const char * user_string);
const char *FdToUsername( int client_fd );
int Mywrite( int fd, const void * buf,unsigned int count );
