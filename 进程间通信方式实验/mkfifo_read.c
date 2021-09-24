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

//命名管道可以通过路径名来指出,并且在文件系统中是可见的
//建立了有名管道之后，两个进程就可以把它当作普通文件一样进行读写操作，使用方便
//数据严格遵循FIFO先进先出原则
#define P_FIFO "/tmp/p_fifo" 
#define max_v 105

int main()
{
  char str[max_v];

  if(access(P_FIFO,F_OK)==0)//管道文件存在
  {
    execlp("rm","-f",P_FIFO,NULL);//删除
    cout<<"the FIFO file have existed,delete it";
  }

//有名管道的创建：int mkfifo（const char* pathname，mode_t mode）

  if(mkfifo(P_FIFO,0777)<0)//创建命名管道，fifo文件
  {
    cout<<"create fifo file error"<<endl;
    return 0;
  }

  int fd=open(P_FIFO,O_RDONLY|O_NONBLOCK);//打开fifo文件

  while(1)
  {
    memset(str,0,sizeof(str));//清空
    int num=read(fd,str,max_v);//读取数据
    if(num==0)
    {
      cout<<"no data"<<endl;
    }else
    {
      str[num]='\0';
      cout<<str<<endl;
    }
    sleep(1);//sleep 1s继续读
  }
  close(fd);//关闭fd
  return 0;
}