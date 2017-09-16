#include "unp.h"
int main(int argc,char**argv){
	int sockfd;
	struct sockaddr_in servaddr;
	if(argc != 2)
		err_quit("usage:tcpcli<IPaddress>");
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	//SOCK_STRREAM为TCP协议，通信协议类型
	bzero(&servaddr,sizeof(servaddr));
	//置零，指针所指的前几位
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	//将整数由主机字节顺序转变成网络字节顺序unp.h中为9877-->SERV_PORT
	inet_pton(AF_INET,argv[1],&servaddr.sin_addr);
	//将点十进制的IP地址，转化为二进制的ip地址，赋给第三个参数的指针所指
	connect(sockfd,(SA*)&servaddr,sizeof(servaddr));
	//连接过程详解，没调用bind(),内核会为套接字选择一个端口
	//服务器调用Listen，accept()，
	//客户用connect()来连接服务器，&servaddr，中含有ip和port连接服务器
	//未bind()时，内核选定源ip和port
	//为tcp协议时，connect()激发了三次握手，成功或者出错才会返回一个值
	str_cli(stdin,sockfd);
	//stdin作为输入的文件指针，int sockfd作为文件标识符
	exit(0);
}
