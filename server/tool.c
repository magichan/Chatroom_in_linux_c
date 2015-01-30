/*
 * =====================================================================================
 *
 *       Filename:  tool.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2015年01月30日 16时40分20秒
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
#include"tool.h"
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  MyError
 *  Description:  输出错误并且退出
 *        Entry:  错误描述，函数，位置
 *         Exit:  void 
 * =====================================================================================
 */
void MyError(char * err_string,const char * function, int line)
{
        fprintf(stderr,"[function:%s,line:%d]",function,line);
        perror(err_string);
        exit(1);

}
void MyErrorPthread(char * err_string,const char * function, int line)
{
        fprintf(stderr,"[function:%s,line:%d]",function,line);
        perror(err_string);
        pthread_exit(0);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Log
 *  Description:  建立日志文件 logfile ,不存在则创建，按照
 *                时间 事件 人物 的格式写入
 *        Entry:  操作和使用者
 *         Exit:  空
 * =====================================================================================
 */
void Log(char *log_string,char * user_string)
{   
      
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  FdToUsername
 *  Description:  根据套接字，找到用户名
 *        Entry:  套接字
 *         Exit:  用户名
 * =====================================================================================
 */
const char * FdToUsername( int client_fd )
{

}

