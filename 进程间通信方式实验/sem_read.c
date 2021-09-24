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
#include<sys/msg.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<sys/types.h>
#include<sys/ipc.h>

using namespace std;

#define sem_key 4001
#define shm_key 5678
//信号量+共享内存，共享内存解决进程间的通信，而信号量解决共享内存的同步问题

union semnu
{
  int val;
};

int main()
{
  //创建共享内存
  int shmid=shmget(shm_key,sizeof(int),IPC_CREAT|0666);
  if(shmid<0)
  {
    cout<<"create shm error";
    return -1;
  }

  //将共享内存链接到当前进程的地址空间
  void *shmptr=shmat(shmid,NULL,0);
  if(shmptr==(void*)-1)
  {
    cout<<"shmat error:"<<strerror(errno)<<endl;
    return -1;
  }

  int *data=(int*)shmptr;
  int semid=semget(sem_key,2,IPC_CREAT|0666);//创建信号量

  union semnu s;

  struct sembuf sbuf;
  struct sembuf sb[1] = {{0, -1, 0}};

  while(1)
  {
    semop(semid, sb, 1);//信号量减1，当信号量<0则会阻塞等待写入端写入数据，>0则输出数据
    cout<<"the NUM="<<*data<<endl;
  }
  return 0;
}