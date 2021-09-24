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

#define sem_key 4001//信号量key
#define shm_key 5678//共享内存key

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
  //创建和打开信号量：semget函数
  
  int semid=semget(sem_key,2,0666);//创建信号量

  union semnu s;
  struct sembuf sbuf;
  struct sembuf sb[1] = {{0, 1, 0}};

  while(1)
  {
    //sleep(1);
    cin>>*data;//往共享内存写入数据
	
//改变信号量的值：semop函数 int semop(int semid,struct sembuf *sops,unsigned nsops);	

// sem_id:信号量标识符
// *sops：指向存储信号操作结构的数组指针
// nsops:信号操作结构的数量，>=1

    semop(semid, sb, 1);//信号量+1
  }
  return 0;
}