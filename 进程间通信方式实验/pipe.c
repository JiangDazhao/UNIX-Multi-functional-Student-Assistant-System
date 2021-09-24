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

#define input 0
#define output 1

int main()
{
  int fd[2];
  pid_t pid;
  //pipe()函数创建无名管道，fd[0]为读端，fd[1]为写端
  pipe(fd);
  pid=fork();


  //pid==0子进程写，关闭子进程写端，pid>0父进程读,关闭父进程读端
  if(pid<0)
  {
    cout<<"fork error"<<endl;
    return 0;
  }else if(pid==0)
  {
	
    cout<<"in child process:write to parent"<<endl;
    close(fd[input]);
    char str[]="hello,my parents";
    write(fd[output],str,strlen(str));
    exit(0);
  }else
  {
	 usleep(3);
    cout<<"in parent process:"<<endl;
    close(fd[output]);
    char str[256];
    int num=read(fd[input],str,sizeof(str));
    str[num]='\0';
    cout<<num<<" bytes from child, the content is:"<<str<<endl;
    exit(0);
  }
  return 0;
}
