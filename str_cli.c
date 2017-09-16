#include "unp.h"

void str_cli(FILE *fp,int sockfd)
{
	char sendline[MAXLINE],recvline[MAXLINE];
	//字符串最大的数组
	while(fgets(sendline,MAXLINE,fp)!=NULL){
		//sendline作为字符型指针，指向存储读入数据的缓冲区地址
		//fp作为读取的流
		writen(sockfd,sendline,strlen(sendline));
		//按照文件的操作方式操作sockfd接口
		//sendline缓冲区作为发出的缓冲区
		if(readline(sockfd,recvline,MAXLINE)==0)
			err_quit("str_cli:server terminanted prematurely");
		//readline()作用
		fputs(recvline,stdout);
		//stdout是一个文件指针是一个标准输出
	}
}
