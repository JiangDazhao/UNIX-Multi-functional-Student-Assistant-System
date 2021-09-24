#include"header.h"
#define CALPORT_PROCESS  8999	//计算器的服务器端口号
#define LISQUEUE   20	//服务器监听的队列长度
#define BUFFER_SIZE 1024	//读写信息缓冲区长度


char sendbuf[BUFFER_SIZE]={0};
char recvbuf[BUFFER_SIZE]={0};

int connfd;///和客户端的连接
int server_sockfd;///定义sockfd


void main(int argc,char *argv[]) {
    ///定义sockfd
    server_sockfd = socket(AF_INET,SOCK_STREAM, 0);

    ///定义sockaddr_in
    struct sockaddr_in server_sockaddr;
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(CALPORT_PROCESS);
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    ///bind，成功返回0，出错返回-1
    if(bind(server_sockfd,(struct sockaddr *)&server_sockaddr,sizeof(server_sockaddr))==-1) {
        perror("Bind error happens!!!");
        exit(1);
    }

    ///listen，成功返回0，出错返回-1
    if(listen(server_sockfd,LISQUEUE) == -1) {
        perror("Listen error happens!!!");
        exit(1);
    }
	
	printf("Waiting for connection!!!\n");

	
	while(1){
		int n;
		//客户端套接字
		struct sockaddr_in client_addr;
		socklen_t length = sizeof(client_addr);

		//处理客户端连接，成功返回非负描述字，出错返回-1
		connfd = accept(server_sockfd, (struct sockaddr*)&client_addr, &length);
		
		memset(recvbuf,0,sizeof(recvbuf));
		memset(sendbuf,0,sizeof(sendbuf));
		
		int pid = fork();
        if(pid == 0){  //创建一个子进程处理数据
            close(server_sockfd);//关闭子进程端的监听        
            while(1){
                /*从connfd文件描述符指向的文件读取数据*/
                n  = read(connfd, recvbuf, sizeof(recvbuf));
                if(n == 0){
                    printf("No recv from client!!!\n");
					break;
                }
                printf("recv msg:%s\n",recvbuf);

                /*将buf转化为大写后写入connfd*/        
                double a = 0, b = 0;
				char c;
				sscanf(recvbuf, "%lf%c%lf", &a, &c, &b);
 
				if ('+' == c) {
					sprintf(sendbuf,"%lf", a+b);
				}	
				else if ('-' == c) {
					sprintf(sendbuf, "%lf", a-b);
				}
				else if ('*' == c) {
					sprintf(sendbuf, "%lf", a*b);
				}
				else if ('/' == c) {
					sprintf(sendbuf, "%lf", a/b);
				}	
				printf("sendMsg=%s\n",sendbuf);
				write(connfd, sendbuf, strlen(sendbuf));
				}    
				close(connfd);//处理完毕关闭连接
				exit(0);
        }else if(pid > 0){ //父进程
            close(connfd);//任务交由子进程处理，父进程关闭
        }else{
            printf("Fork error happens!!!\n");
        }

        if(n == 0 ||  n == -1){
            break;
        }
    }
    return 0;
		
		
		

}

			
 

