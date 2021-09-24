#include"header.h"
#define CALPORT_THREAD  9000	//计算器的服务器端口号
#define LISQUEUE   20	//服务器监听的队列长度
#define BUFFER_SIZE 1024	//读写信息缓冲区长度
#define THREAD_POOL_SIZE      ( ( const int )20 )//线程池大小

static volatile int g_run = 1;
static volatile int g_pool_run = 1;
static volatile int g_send = 0;

static pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t g_cond = PTHREAD_COND_INITIALIZER;

char sendbuf[BUFFER_SIZE]={0};
char recvbuf[BUFFER_SIZE]={0};

int connfd;///和客户端的连接
int server_sockfd;///定义sockfd

static void sig_handler( int sig )	//terminate外部信号关闭线程池
{
	if ( sig == SIGTERM )g_run = 0;
}

static void task_dispatch( )  //任务分发函数
{
		pthread_mutex_lock( &g_mutex );
		g_send ++;
	//	printf("dispatch_gsend:%d\n",g_send);
		pthread_mutex_unlock( &g_mutex );
		//告诉所有线程有任务来了，唤醒所有线程
		pthread_cond_broadcast( &g_cond );
		usleep( 10 );
}

void* thr_routine( void* arg )	//计算机线程处理函数
{
	//客户端套接字
	struct sockaddr_in client_addr;
	socklen_t length = sizeof(client_addr);

	//处理客户端连接，成功返回非负描述字，出错返回-1
	connfd = accept(server_sockfd, (struct sockaddr*)&client_addr, &length);
	if(connfd<0) {
		perror("Connect error happens!!!");
		exit(1);
	}
	else{
		task_dispatch();
	}
		
	while ( 1 )
	{	
		pthread_mutex_lock( &g_mutex );
		
		while ( g_pool_run == 1 && g_send == 0 )
		{
			/*线程池在工作，在没有任务时，所有线程一直等待条件g_cond信号*/
			/*当某个任务来了以后，收到g_cond信号，g_send!=0，*/
			pthread_cond_wait( &g_cond, &g_mutex );
		}
		//此处仍然持有锁
		/*控制线程池退出*/
		if ( g_pool_run == 0 )
		{
			//将持有的锁释放
			pthread_mutex_unlock( &g_mutex );
			break;
		}
		/*执行一次消灭了一个发送任务,必须在此处还持有锁的时候-1*/
		g_send --;
		pthread_mutex_unlock( &g_mutex );
		
		memset(recvbuf,0,sizeof(recvbuf));
		memset(sendbuf,0,sizeof(sendbuf));
		
        if(0!=read(connfd, recvbuf, sizeof(recvbuf)))
		{
			printf("recv msg:%s\n",recvbuf);
			 
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
			close(connfd);	
		}
       else
	   {
		   printf("No recv from client!!!\n");
		   exit(1);
	   }
		close(server_sockfd);
		usleep( 20000 );
		//printf("solved_gsend:%d\n",g_send);
	}

	return NULL;
}


void main(int argc,char *argv[]) {
    ///定义sockfd
    server_sockfd = socket(AF_INET,SOCK_STREAM, 0);

    ///定义sockaddr_in
    struct sockaddr_in server_sockaddr;
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(CALPORT_THREAD);
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

	int i, j;
	pthread_t tid[THREAD_POOL_SIZE];
	/*发送和sig_handler有关的信号，主线程退出*/
	signal( SIGTERM, sig_handler );

	g_run = 1;
	g_send = 0;

	for ( i = 0; i < THREAD_POOL_SIZE; i ++ )
	{
		/*若在创建线程过程中收到pthread_create返回的错误码,线程池构建失败*/
		if ( pthread_create( tid + i, NULL, thr_routine, &i ) != 0 )
		{
			//创建一半就挂了，直接就置0
			g_run = 0;
			//不知道多少线程被创建，广播唤醒所有阻塞信号
			pthread_cond_broadcast( &g_cond );
			//收尸函数将所有已经创建的线程回收
			for ( j = 0; j < i; j ++ )pthread_join( tid[j], NULL );
			return;
		}
	}

	while ( g_run )usleep( 10000 );

	g_pool_run = 0;
	pthread_cond_broadcast( &g_cond );
	for ( i = 0; i < THREAD_POOL_SIZE; i ++ )pthread_join( tid[i], NULL );

	printf( "System Will Quit ...\n" );


}

			
 

