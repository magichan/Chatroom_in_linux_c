/*
 * =====================================================================================
 *
 *       Filename:  frame.h
 *
 *    Description:  由服务器向客户端传送数据的帧
 *
 *        Version:  1.0
 *        Created:  2015年01月28日 17时58分49秒
 *       Revision:  none
 *       Compiler:  clang 
 *
 *         Author:  magichan (fgm), 574932286@qq.com
 *   Organization:  FH Südwestfalen, Iserlohn
 *
 * ===================================================================================
 */

#include<time.h>
#ifndef _FRAME_H_
#define _FRAME_H_

#define USER_MAX 30//存放数据
#define MESG_MAX 1024

struct SerToCliFrame{ /* 服务端向客户端发送 */
        int chatroom_authority;            // 客户端权限
        int option;                       // 命名选项 
        time_t send_time;                 // 发送时间
        char sender_name[USER_MAX];       // 发送者信息
        char mesg_data[MESG_MAX];        // 数据
};

/*   结构体Frame 会用到的状态宏定义  */

#define CLIENT_STATUS_ROOT               1 /* 管理员权限 */
#define CLIENT_STATUS_COMMON             2 /* 普通用户权限 */
#define CLIENT_STATUS_AUDIENCE           3 /* 旁听用户权限 */

struct CliToSerFrame{                      /* 客户端向服务端发送  */
        int option;                        /* 命令选项，仅和登录和注册有关 */
        char target_name[USER_MAX];
        char mesg_data[MESG_MAX];          /* 数据 */
};
/*  登陆和注册 退出  所用 宏定义  */
#define REGISTER_SUCCSE            4 /* 注册成功，要求客户端退出*/
#define REQUIE_REGISTER            5 /* 客户端注册请求 */
#define REGISTER_USERNAME          6 /* 注册名 */
#define REGISTER_USERPASSWORD      7 /* 注册密码 */
#define REQUEST_LOGIN              8 /* 客户端登录请求 */
#define LOGIN_USERNAME             9 /* 登录名 */
#define LOGIN_USERPASSWORD         10 /* 登录密码  */
#define LOGIN_SUCCESE              11 /* 登陆成功开始回话*/
#define REQUEST_EXIT               12/*客户端退出请求||服务器要求退出*/
/*   get mesg */
#define SEND_MESG                  13 /* 发送信息 */
#define SEND_COMD                  14 /* 发送命令 */
#define USER_LIST_MESG             15 /* 登陆者列表信息*/
#define CHANGE_AUTHORITY           16 /* 改变服务端权限*/
#define SEND_PRIVATE_MESG          17 /* 发送私人消息 */
#endif
