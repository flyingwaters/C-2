#include    "unp.h"  
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
readline(int fd, void *vptr, size_t maxlen)  
{  
    ssize_t n, rc;  
    char    c, *ptr;  
  
    ptr = vptr;  
    for (n = 1; n < maxlen; n++) {  
        if ( (rc = my_read(fd, &c)) == 1) {  
            *ptr++ = c;  
            if (c == '\n')  
                break;  /* newline is stored, like fgets() */  
        } else if (rc == 0) {  
            *ptr = 0;  
            return(n - 1);  /* EOF, n - 1 bytes were read */  
        } else  
            return(-1);     /* error, errno set by read() */  
    }  
  
    *ptr = 0;   /* null terminate like fgets() */  
    return(n);  
}  
  
ssize_t  
readlinebuf(void **vptrptr)  
{  
    if (read_cnt)  
        *vptrptr = read_ptr;  
    return(read_cnt);  
}  
/* end readline */  
  
ssize_t  
Readline(int fd, void *ptr, size_t maxlen)  
{  
    ssize_t     n;  
  
    if ( (n = readline(fd, ptr, maxlen)) < 0)  
        err_sys("readline error");  
    return(n);  
}  

