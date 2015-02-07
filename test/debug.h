/*
 * =====================================================================================
 *
 *       Filename:  debug.h
 *
 *    Description:  debug  的宏定义
 *
 *        Version:  1.0
 *        Created:  2015年01月29日 19时07分52秒
 *       Revision:  none
 *       Compiler:  clang 
 *
 *         Author:  magichan (fgm), 574932286@qq.com
 *   Organization:  FH Südwestfalen, Iserlohn
 *
 * =====================================================================================
 */
#define _DEBUG_

#ifndef _DEBUG_
#define debug_msg(fmt,args...)  printf("OK");
#define input_msg(fmt,args...)  printf("OK");
#else
#define debug_msg(fmt,args...)   printf("[%s:%s:line %d]"fmt,__FILE__,__FUNCTION__,__LINE__,##args)
#define input_msg(fmt,args...)   printf("[%s:%s:line %d]"fmt,__FILE__,__FUNCTION__,__LINE__,##args)
#endif
