#include <iostream>
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<semaphore.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/stat.h>
using namespace std;

#define P_FIFO "/tmp/p_fifo"
#define max_v 105

int main()
{

    int fd=open(P_FIFO,O_WRONLY|O_NONBLOCK);//打开fifo文件
    char str[]="give you data";
    str[strlen(str)]='\n';

    write(fd,str,strlen(str));//写入数据
    sleep(1);

    close(fd);
    return 0;
}