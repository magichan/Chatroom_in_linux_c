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
void Log(const char *log_string, const char * user_string)
{   
        FILE * log_fp;
        time_t time_now = time(NULL);        

        if( (log_fp=fopen("log_fp","a+")) == NULL )
        {
                MyError("fopne",__FUNCTION__,__LINE__ );
        }
        fprintf(log_fp,"time:%s what:%10s   who:%10s\n",ctime(&(time_now)),log_string,user_string);
        fclose(log_fp);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  get_info
 *  Description:  获取一行输入,以回车为结尾，在buf中删除\n,以\0为结尾
 *        Entry:  存放空间 buf ,空间大小
 *         Exit:  成功存储返回 0 ， buf空间不存，存储失败在返回 -1 ， 字符串过长返回 -2
 * =====================================================================================
 */
int   GetInfo( char *buf,unsigned int counnt)
{
        int  i = 0;
        int  c;
        int temp;

        if( buf == NULL )
        {
                return -1;
        }
         while( (temp=getchar()) == '\n') ;
        
        buf[i++] = temp;

         while((buf[i]=getchar()) != '\n')
         {
                 i++;
         }
        if( i >= counnt )
        {
                memset(buf,0,counnt);
                return -2;
        }

         buf[i] = '\0';//去回车，加\0
         return 0;

}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Myfflush
 *  Description:  清空标准输入流
 *        Entry:
 *         Exit:
 * =====================================================================================
 */
void Myfflush( void )
{
        char ch;
        while( (ch=getchar())!='\n'&&ch!=EOF);
}

