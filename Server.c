#include"header.h"
#include"set_lock.c"
#include"login.c"
#define BUFFER_SIZE 256	//读写信息缓冲区长度
#define CHATPORT  9001	//聊天室的服务器端口号
#define THREAD_POOL_SIZE      ( ( const int )20 )//线程池大小
int clients[THREAD_POOL_SIZE]={0};
char buff[BUFFER_SIZE];


void* clientconnect(void* arg);

int serverSocket;	//声明服务端套接字


int main(int argc,char** argv){
	
	//输出消息记录
     char buff3[BUFFER_SIZE];
     int fd1;
     fd1 = open("logMessage.txt", O_RDONLY | O_CREAT | O_APPEND, 0644);
     if(fd1 < 0){
            printf("打开文件失败\n");			
            exit(1);
     }
	 //通过lseek函数将文件fd1里的记录输出
    lseek(fd1,SEEK_SET,0);
    printf("消息记录为:\n");
    int readi;    
    while((readi=read(fd1,buff3,BUFFER_SIZE))>0){
        printf("%s",buff3);
        printf("消息记录结束，以下为聊天内容:\n"); 
        memset(buff3,0,sizeof(buff3));
    }     
    close(fd1);    
	
	//printf("声明前无错误\n");

    //地址及数据存储顺序处理(声明服务端地址信息结构体)
    struct sockaddr_in serveraddr;
    //每一个socket都用一个网络结构描述{协议、本地地址、本地端口}来表示
    //socket函数用于建立一个套接字，同时指定协议和类型  
    serverSocket=socket(AF_INET,SOCK_STREAM,0);
    //初始化一下地址信息结构体
    memset(&serveraddr,0,sizeof(struct sockaddr_in));    
    serveraddr.sin_family = AF_INET;//设置地址族
    serveraddr.sin_port = htons(CHATPORT);//设置服务器端口号(htons()2字节主机字节序转换为网络字节序)
    //设置地址信息中的IP地址,并格式转化
    inet_pton(AF_INET,"127.0.0.1",&(serveraddr.sin_addr.s_addr));
    //serverSocket绑定serveraddr
    if(bind(serverSocket,(struct sockaddr*)&serveraddr,sizeof(serveraddr))==-1){
        perror("绑定地址:");
        return -1;
    }
    //设置listen监听的长度(客户端的个数)
    listen(serverSocket,THREAD_POOL_SIZE);//设置为20个
	
	
	int i,j;
	pthread_t tid[THREAD_POOL_SIZE];
	
	//printf("listn无错误\n");
	//创建线程池
	for ( i = 0; i < THREAD_POOL_SIZE; i ++ )
	{
		pthread_create(tid+i,NULL,clientconnect,&i);
	}
	
	//printf("pthread_create无错误\n");
	for ( i = 0; i < THREAD_POOL_SIZE; i ++ )
	{
		 pthread_join(tid[i],NULL);
	}

    Map_delete();
    return 0 ;
}

void* clientconnect(void* arg){
        
		//输出时间
        struct timeval tv;
        gettimeofday(&tv,NULL);//获取1970-1-1到现在的时间结果保存到tv中
        uint64_t sec=tv.tv_sec;
        uint64_t min=tv.tv_sec/60;
        struct tm cur_tm;//保存转换后的时间结果
        localtime_r((time_t*)&sec,&cur_tm);
        char cur_time[20];
        snprintf(cur_time,20,"%d-%02d-%02d %02d:%02d:%02d",cur_tm.tm_year+1900,cur_tm.tm_mon+1,cur_tm.tm_mday,cur_tm.tm_hour,cur_tm.tm_min,cur_tm.tm_sec);
	// printf("时间无错误\n");
     char buff[BUFFER_SIZE];
	 //printf("转换无错误\n");
	 
	 struct sockaddr_in clientaddr;//客户端的套接字
	 socklen_t clientaddrlen=sizeof(clientaddr);
    int connfd = accept(serverSocket,(struct sockaddr*)&clientaddr,&clientaddrlen);
     //printf("accept无错误\n");
	 
	 for(int i = 0 ; i<THREAD_POOL_SIZE; i++ ){
        if(clients[i] == 0){
            clients[i] = connfd;
            break;
         }
     }
	// printf("client指定无错误\n");
     int filesize;
     int fd;
     while(1)
	 { 
            if( connfd!=-1){	
               fd = open("logMessage.txt", O_RDWR | O_CREAT | O_APPEND, 0644);
                   if(fd < 0){
                   	printf("打开文件失败\n");			
                   exit(1);
                 }                
                //接受客户端传过来的信息  
                recv(connfd,buff,BUFFER_SIZE,0);
                //存入map
                if(strstr(buff,"上线了!!!")!=NULL){
                      Map_set(buff,connfd);                   
                }
                //私聊
                if(strstr(buff,"@")!=NULL){
                    char *username[2];
                    memset(username, 0x0, sizeof(username));
                    split(username, buff, "@");
                    strcat(username[1],"上线了!!!"); 
                      if(Map_get(username[1])!=NULL){
        	              int clisocket = atoi(Map_get(username[1]));
                         send(clisocket,buff,BUFFER_SIZE,0);//私聊发消息  
	               }               
                }else{
                     if(strncmp(buff,"over",4)==0){
                          printf("用户离开了群聊!!!\n");
                           break;                         
                        }
                        if(buff[0]!=0){
                             printf("%s\n",buff);
                             for(int i= 0  ; i < THREAD_POOL_SIZE ; i++ ){
                                  if(clients[i]!=0&&clients[i]!=connfd){
                                     send(clients[i],buff,BUFFER_SIZE,0);                           
                                  }
                             }
                        }     
                }

               
					set_lock(fd, F_WRLCK);//给消息记录文件上写入锁  
//                      strcat(buff,"\n"); 
                    if((filesize=write(fd,cur_time,strlen(cur_time)))<0){//写一个字符到文件中
                        printf("再见，我走了！\n");
                        break;  
                    }else if((filesize=write(fd,buff,strlen(buff)))<0){
                        printf("再见，我走了！\n");
                        break; 
                    }else{
                       char bufff[BUFFER_SIZE];
                       strcat(bufff,"\n");
                       write(fd,bufff,sizeof(bufff));
                       memset(bufff,0,sizeof(bufff));  
                    }
                   set_lock(fd, F_UNLCK); //给文件解锁 
                   close(fd);                          
                   memset(buff,0,BUFFER_SIZE);  
            }
			else{
                perror("连接");
                exit(-1);                
            }
	  }

    close(fd);  
	
    close(connfd);
    close(serverSocket);  
	}


