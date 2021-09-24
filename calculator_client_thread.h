#include "header.h"
#define CALPORT_THREAD  9000	//�������ķ������˿ں�
#define BUFFER_SIZE 1024	//��д��Ϣ����������

//���������Ϣ�ͷ�����Ϣ�Ļ�����
char sendbuf[BUFFER_SIZE]={0};
char recvbuf[BUFFER_SIZE]={0};
 
void deal_num(GtkButton *button, gpointer data)
{
	const char *text = gtk_button_get_label(button);
 
	//��ղ���
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
	//�����ͷ��������ӣ���������ĵ�ʽ���������Ĳ��� (0 == strcmp("=", text))
	else 
	{	
		//�����ͷ����������׽���
		int client_socket = socket(AF_INET, SOCK_STREAM, 0);   
		if(client_socket == -1)
		{
			perror("Socket error happens!!!!\n");
			exit(1);
		}
	
		//��ʼ����������ַ�ṹ��
		struct sockaddr_in servaddr;
		memset(&servaddr, 0, sizeof(servaddr));
	
		//�ƶ��ṹ���е�Э���塢�˿ںš���ip��ַ
		servaddr.sin_family = AF_INET;  /* Internet��ַ�� */
		servaddr.sin_port = htons(CALPORT_THREAD);  /* �˿ںţ�htons�ı��ֽ��� */
		servaddr.sin_addr.s_addr = htonl(INADDR_ANY);   /* IP��ַ���ı��ֽ���*/
		inet_aton("127.0.0.1", &(servaddr.sin_addr));//�ַ���ip��ַת��Ϊ��������ip��ַ
 
		//���׽������ӷ�����
		int addrlen = sizeof(servaddr);
		int listen_socket =  connect(client_socket,(struct sockaddr *)&servaddr,addrlen);  //���ӷ�����
		if(listen_socket == -1)
		{
			perror("Socket connect error happens!!!\n");
			exit(1);
		}
	
		else printf("Succesfully connect to server!!!\n");
		
		//while(1)
		//{			
		//write�������������Ϣ���й���ʽ�Ĵ���sendbuf
		printf("sendmsg=%s\n",sendbuf);
		write(client_socket, sendbuf, strlen(sendbuf));
		//read���ܷ��������ص���Ϣ��recvbuf
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
	 //1.gtk������ʼ��
	 gtk_init(&argc, &argv);
 
	 //2.����һ������
	 GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	 //���ñ���
	 gtk_window_set_title(GTK_WINDOW(window), g_convert("�򵥼�����",-1,"UTF-8","GB2312",NULL,NULL,NULL));

	 //3.����һ���������5��4��
	 GtkWidget *table = gtk_table_new(5,4,TRUE);
	 //���ô�С
	 gtk_window_set_default_size(GTK_WINDOW(window) , 400 , 300); 
	 //��table���뵽window��
	 gtk_container_add(GTK_CONTAINER(window), table);
	 //���м�
	 gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	 
	 //4.����һ���б༭
	 GtkWidget *entry = gtk_entry_new();
	 //�����б༭������
	 gtk_entry_set_text(GTK_ENTRY(entry), g_convert("������Ҫ�����ʽ�ӣ�",-1,"UTF-8","GB2312",NULL,NULL,NULL));
	 //�����б༭������༭��ֻ����ʾ��
	 gtk_editable_set_editable(GTK_EDITABLE(entry), FALSE);
 
	 //5.���������ť
	 GtkWidget *button0 = gtk_button_new_with_label("0");//��ֵ��0
	 GtkWidget *button1 = gtk_button_new_with_label("1");//��ֵ��1
	 GtkWidget *button2 = gtk_button_new_with_label("2");//��ֵ��2
	 GtkWidget *button3 = gtk_button_new_with_label("3");//��ֵ��3
	 GtkWidget *button4 = gtk_button_new_with_label("4");//��ֵ��4
	 GtkWidget *button5 = gtk_button_new_with_label("5");//��ֵ��5
	 GtkWidget *button6 = gtk_button_new_with_label("6");//��ֵ��6
	 GtkWidget *button7 = gtk_button_new_with_label("7");//��ֵ��7
	 GtkWidget *button8 = gtk_button_new_with_label("8");//��ֵ��8
	 GtkWidget *button9 = gtk_button_new_with_label("9");//��ֵ��9
 
	 GtkWidget *button_add = gtk_button_new_with_label("+");//�Ӻ�
	 GtkWidget *button_minus = gtk_button_new_with_label("-");//����
	 GtkWidget *button_mul = gtk_button_new_with_label("*");//�˺�
	 GtkWidget *button_div = gtk_button_new_with_label("/");//����
	 GtkWidget *button_equal = gtk_button_new_with_label("=");//�Ⱥ�
	 GtkWidget *button_delete = gtk_button_new_with_label("c");//�˸��
 
	 //6.���ֽ�����İ�ť������table������
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
 
	 //7.ע���źź�������entry�����ص�����deal_num()
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
 
	 //7.��ʾ���пؼ�
	 gtk_widget_show_all(window);
 
	 //8.���¼�ѭ��
	 gtk_main();
 
 }