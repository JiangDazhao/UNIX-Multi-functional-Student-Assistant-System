#include "header.h"
#define CALPORT_THREAD  9000	//计算器的服务器端口号
#define BUFFER_SIZE 1024	//读写信息缓冲区长度

//定义接受信息和发送信息的缓冲区
char sendbuf[BUFFER_SIZE]={0};
char recvbuf[BUFFER_SIZE]={0};
 
void deal_num(GtkButton *button, gpointer data)
{
	const char *text = gtk_button_get_label(button);
 
	//清空操作
	if (0 == strcmp("c",text)) {
		int len=strlen(sendbuf);
		for(int i=0;i<len;i++)
			sendbuf[i] = 0;
		gtk_entry_set_text(GTK_ENTRY(data), sendbuf);
	}
	else if(0!= strcmp("=", text))
	{
		strcat(sendbuf,text);
		gtk_entry_set_text(GTK_ENTRY(data), sendbuf);
	}
	//创建和服务器连接，发送输入的等式给服务器的操作 (0 == strcmp("=", text))
	else 
	{	
		//创建和服务器连接套接字
		int client_socket = socket(AF_INET, SOCK_STREAM, 0);   
		if(client_socket == -1)
		{
			perror("Socket error happens!!!!\n");
			exit(1);
		}
	
		//初始化服务器地址结构体
		struct sockaddr_in servaddr;
		memset(&servaddr, 0, sizeof(servaddr));
	
		//制定结构体中的协议族、端口号、和ip地址
		servaddr.sin_family = AF_INET;  /* Internet地址族 */
		servaddr.sin_port = htons(CALPORT_THREAD);  /* 端口号，htons改变字节序 */
		servaddr.sin_addr.s_addr = htonl(INADDR_ANY);   /* IP地址，改变字节序*/
		inet_aton("127.0.0.1", &(servaddr.sin_addr));//字符串ip地址转换为网络序列ip地址
 
		//将套接字连接服务器
		int addrlen = sizeof(servaddr);
		int listen_socket =  connect(client_socket,(struct sockaddr *)&servaddr,addrlen);  //连接服务器
		if(listen_socket == -1)
		{
			perror("Socket connect error happens!!!\n");
			exit(1);
		}
	
		else printf("Succesfully connect to server!!!\n");
		
		//while(1)
		//{			
		//write向服务器发送消息，有关算式的存在sendbuf
		printf("sendmsg=%s\n",sendbuf);
		write(client_socket, sendbuf, strlen(sendbuf));
		//read接受服务器发回的消息到recvbuf
		if(0!= read(client_socket, recvbuf, BUFFER_SIZE))
		{
			printf("recvbuf = %s\n", recvbuf);
			gtk_entry_set_text(GTK_ENTRY(data), recvbuf);
		}
		else if(0==read(client_socket, recvbuf, BUFFER_SIZE))
		{
			printf("No msg from server!!!");
			exit(0);
		}
		else {
			printf("Read from server error happens!!!");
			exit(1);
		}
		
		memset(recvbuf,0,sizeof(recvbuf));
		memset(sendbuf,0,sizeof(sendbuf));
		//}
		
		
		close(client_socket);		
	}
}
 
void calclient_thread(int argc,char *argv[])
 {
	 //1.gtk环境初始化
	 gtk_init(&argc, &argv);
 
	 //2.创建一个窗口
	 GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	 //设置标题
	 gtk_window_set_title(GTK_WINDOW(window), g_convert("简单计算器",-1,"UTF-8","GB2312",NULL,NULL,NULL));

	 //3.创建一个表格容器5行4列
	 GtkWidget *table = gtk_table_new(5,4,TRUE);
	 //设置大小
	 gtk_window_set_default_size(GTK_WINDOW(window) , 400 , 300); 
	 //将table加入到window中
	 gtk_container_add(GTK_CONTAINER(window), table);
	 //放中间
	 gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	 
	 //4.创建一个行编辑
	 GtkWidget *entry = gtk_entry_new();
	 //设置行编辑的内容
	 gtk_entry_set_text(GTK_ENTRY(entry), g_convert("请输入要计算的式子：",-1,"UTF-8","GB2312",NULL,NULL,NULL));
	 //设置行编辑不允许编辑，只能显示用
	 gtk_editable_set_editable(GTK_EDITABLE(entry), FALSE);
 
	 //5.创建多个按钮
	 GtkWidget *button0 = gtk_button_new_with_label("0");//数值键0
	 GtkWidget *button1 = gtk_button_new_with_label("1");//数值键1
	 GtkWidget *button2 = gtk_button_new_with_label("2");//数值键2
	 GtkWidget *button3 = gtk_button_new_with_label("3");//数值键3
	 GtkWidget *button4 = gtk_button_new_with_label("4");//数值键4
	 GtkWidget *button5 = gtk_button_new_with_label("5");//数值键5
	 GtkWidget *button6 = gtk_button_new_with_label("6");//数值键6
	 GtkWidget *button7 = gtk_button_new_with_label("7");//数值键7
	 GtkWidget *button8 = gtk_button_new_with_label("8");//数值键8
	 GtkWidget *button9 = gtk_button_new_with_label("9");//数值键9
 
	 GtkWidget *button_add = gtk_button_new_with_label("+");//加号
	 GtkWidget *button_minus = gtk_button_new_with_label("-");//减号
	 GtkWidget *button_mul = gtk_button_new_with_label("*");//乘号
	 GtkWidget *button_div = gtk_button_new_with_label("/");//除号
	 GtkWidget *button_equal = gtk_button_new_with_label("=");//等号
	 GtkWidget *button_delete = gtk_button_new_with_label("c");//退格键
 
	 //6.布局将上面的按钮均放入table容器中
	 gtk_table_attach_defaults(GTK_TABLE(table), entry, 0, 4, 0, 1);
 
	 gtk_table_attach_defaults(GTK_TABLE(table), button0, 0, 1, 4, 5);
	 gtk_table_attach_defaults(GTK_TABLE(table), button1, 0, 1, 3, 4);
	 gtk_table_attach_defaults(GTK_TABLE(table), button2, 1, 2, 3, 4);
	 gtk_table_attach_defaults(GTK_TABLE(table), button3, 2, 3, 3, 4);
	 gtk_table_attach_defaults(GTK_TABLE(table), button4, 0, 1, 2, 3);
	 gtk_table_attach_defaults(GTK_TABLE(table), button5, 1, 2, 2, 3);
	 gtk_table_attach_defaults(GTK_TABLE(table), button6, 2, 3, 2, 3);
	 gtk_table_attach_defaults(GTK_TABLE(table), button7, 0, 1, 1, 2);
	 gtk_table_attach_defaults(GTK_TABLE(table), button8, 1, 2, 1, 2);
	 gtk_table_attach_defaults(GTK_TABLE(table), button9, 2, 3, 1, 2);
 
	 gtk_table_attach_defaults(GTK_TABLE(table), button_add, 1, 2, 4, 5);
	 gtk_table_attach_defaults(GTK_TABLE(table), button_minus, 2, 3, 4, 5);
	 gtk_table_attach_defaults(GTK_TABLE(table), button_mul , 3, 4, 2, 3);
	 gtk_table_attach_defaults(GTK_TABLE(table), button_div, 3, 4, 3, 4);
	 gtk_table_attach_defaults(GTK_TABLE(table), button_equal, 3, 4, 4, 5);
	 gtk_table_attach_defaults(GTK_TABLE(table), button_delete, 3, 4, 1, 2);
 
	 //7.注册信号函数，把entry传给回调函数deal_num()
	 g_signal_connect(button0, "pressed", G_CALLBACK(deal_num), entry);
	 g_signal_connect(button1, "pressed", G_CALLBACK(deal_num), entry);
	 g_signal_connect(button2, "pressed", G_CALLBACK(deal_num), entry);
	 g_signal_connect(button3, "pressed", G_CALLBACK(deal_num), entry);
	 g_signal_connect(button4, "pressed", G_CALLBACK(deal_num), entry);
	 g_signal_connect(button5, "pressed", G_CALLBACK(deal_num), entry);
	 g_signal_connect(button6, "pressed", G_CALLBACK(deal_num), entry);
	 g_signal_connect(button7, "pressed", G_CALLBACK(deal_num), entry);
	 g_signal_connect(button8, "pressed", G_CALLBACK(deal_num), entry);
	 g_signal_connect(button9, "pressed", G_CALLBACK(deal_num), entry);
 
	 g_signal_connect(button_add, "pressed", G_CALLBACK(deal_num), entry);
	 g_signal_connect(button_mul, "pressed", G_CALLBACK(deal_num), entry);
	 g_signal_connect(button_div, "pressed", G_CALLBACK(deal_num), entry);
	 g_signal_connect(button_minus, "pressed", G_CALLBACK(deal_num), entry);
	 g_signal_connect(button_equal, "pressed", G_CALLBACK(deal_num), entry);
	 g_signal_connect(button_delete, "pressed", G_CALLBACK(deal_num), entry);
 
	 //7.显示所有控件
	 gtk_widget_show_all(window);
 
	 //8.主事件循环
	 gtk_main();
 
 }