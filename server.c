#include "unp.h"
static int  read_cnt;//刚开始可以置为一个负值（我的理解）  
static char *read_ptr;  
static char read_buf[MAXLINE];  
  
static ssize_t  
my_read(int fd, char *ptr)//每次最多读取MAXLINE个字符，调用一次，每次只返回一个字符  
{  
  
    if (read_cnt <= 0) {  
again:  
        if ( (read_cnt = read(fd, read_buf, sizeof(read_buf))) < 0) {//如果读取成功，返回read_cnt=读取的字符           if (errno == EINTR)  
                goto again;  
            return(-1);  
        } else if (read_cnt == 0)  
            return(0);  
        read_ptr = read_buf;  
    }  
  
    read_cnt--;//每次递减1，直到<0读完，才执行上面if的命令。  
    *ptr = *read_ptr++;//每次读取一个字符，转移一个字符  
    return(1);  
} 
ssize_t
readline(int fd,void *vptr,size_t maxlen)
{
ssize_t n,rc;
char c,*ptr;
ptr= vptr;
for(n=1;n<maxlen;n++)
{	if((rc=my_read(fd,&c))==1)
{
*ptr++ =c;
if(c=='\n')
break;}
else if(rc == 0)
{*ptr=0;
return(n-1);
}
else return(-1);

}
*ptr = 0;
return(n);
}

void str_echo(int sockfd)
{
ssize_t n;
char line[MAXLINE];
for(;;)
{
	if((n=readline(sockfd,line,MAXLINE))==0)
		return;
	//如果有socket中的有string读出来然后加入line最大为MAXLINE
	write(sockfd,line,n);
	//然后写入sockfd中去，将line中的string
}
}

int main(int argc,char**argv)
{
	int listenfd,connfd;
	pid_t childpid;
	/*
	 #ifndef __pid_t_defined
     	typedef __pid_t pid_t;
     # define __pid_t_defined
     #endif
     #define __S32_TYPE int
由此我们终于找到了pid_t的真实定义：实际他就是  int  类型的。
*/
	socklen_t clilen;
	//typedef int socklen_t；
	struct sockaddr_in cliaddr,servaddr;
	//两个struct sockaddr_in cliaddr,servaddr;
	listenfd = socket(AF_INET,SOCK_STREAM,0);
	//文件标识
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);
	bind(listenfd,(SA*)&servaddr,sizeof(servaddr));
	//绑定socket文件标识符和ip与端口号
	listen(listenfd,LISTENQ);
	//listen()将一个进程变为服务器，可以接受其他进程的请求，进而成为一个服务器进程
	//并且把相应的套接字变为被动的套接字
	//参数backlog
	//这个参数 涉及到一些网络的细节。在进程正理一个一个连接请求的时候，可能还存在其它的连接请求。因为TCP连接是一个过程，
	//所以可能存在一种半连接的状态，有时由 于同时尝试连接的用户过多，
	//使得服务器进程无法快速地完成连接请求。如果这个情况出现了，服务器进程希望内核如何处理呢？
	//内核会在自己的进程空间里维护一 个队列以跟踪这些完成的连接但服务器进程还没有接手处理或正在进行的连接，
	//这样的一个队列内核不可能让其任意大，所以必须有一个大小的上限。这个 backlog告诉内核使用这个数值作为上限。
	//毫无疑问，服务器进程不能随便指定一个数值，内核有一个许可的范围。这个范围是实现相关的。很难有某种统一，一般这个值会小30以内。
	//当调用listen之后，服务器进程就可以调用accept来接受一个外来的请求。
	for(;;)
	{
		clilen = sizeof(cliaddr);
		connfd = accept(listenfd,(SA*)&cliaddr,&clilen);
		//功能参数描述
		//accept()系统调用主要用在基于连接的套接字类型，比如SOCK_STREAM和SOCK_SEQPACKET。
		//它提取出所监听套接字的等待连接队列中第一个连接请求，创建一个新的套接字，并返回指向该套接字的文件描述符。
		//新建立的套接字不在监听状态，原来所监听的套接字也不受该系统调用的影响。
		if((childpid = fork())==0)
		{
			close(listenfd);
			//子进程==0进入 if 关闭监听的进程
			str_echo(connfd);
			//调用str_echo(connfd)这个函数
			exit(0);
		}
		close(connfd);
		//父进程进入这个逻辑直接关闭accept()建立的套接字
	}

}
