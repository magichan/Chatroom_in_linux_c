/*
 * =====================================================================================
 *
 *       Filename:  user.h
 *
 *    Description:  用户表的建立和维护
 *
 *        Version:  1.0
 *        Created:  2015年01月28日 21时30分53秒
 *       Revision:  none
 *       Compiler:  clang 
 *
 *         Author:  magichan (fgm), 574932286@qq.com
 *   Organization:  FH Südwestfalen, Iserlohn
 *
 * =====================================================================================
 */


#ifndef _USER_H_
#define _USER_H_
#include"frame.h"
#include"server.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define USERFIFE  "userlist"

struct UserDate{
        int status;                             /* 用户状态 */
        int authority;                          /* 客户端权限*/
        int confd;                              /* 通信接口*/
        char name[USER_MAX];                    /* 用户名*/
        char password[USER_MAX];                /* 密码 */
        struct UserDate * next;                 /*  */
};
/*  结构体 UserData 补充状态  */

#define USER_STATUS_DEALING               2 /*注册处理*/
#define USER_STATUS_UP                    1 /* 上线 */
#define USER_STATUS_DOWN                  0 /* 下线 */

typedef struct UserDate * PtrUserDate;

 extern PtrUserDate g_user_list;
 extern       int g_sock_fd;
PtrUserDate InitUserList( void );               /* 链表初始化 */
int  AddUser( PtrUserDate  list, PtrUserDate Node ); /*  添加用户*/
int  DeleUser( PtrUserDate list, char * name ); /*  删除用户*/
 PtrUserDate  SearchUser( PtrUserDate list, const char * name ); /* 查找用户 */
int  WriteUserList( PtrUserDate list);          /*  写入文件*/
int  UserListIsEmpty( PtrUserDate list);        /*判断是否为空*/





#endif
