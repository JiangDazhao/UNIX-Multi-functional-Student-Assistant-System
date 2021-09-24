#include "header.h"
#include "calendar.h"
#include "calculator_client_thread.h"
#include "calculator_client_process.h"

GtkWidget *window_select; //���˵���ѡ���ܽ���

GtkWidget *vbox;	//��ֱ��������
GtkWidget *hbox;	//ˮƽ��������

//gtk����Ļ��ƣ��ص��������������������ȷ����ť���¼�ִ�еĶ�������������ָ��
void go_homepage_1(GtkWidget *button1, gpointer entry);
void go_homepage_2(GtkWidget *button2, gpointer entry);
void go_homepage_3(GtkWidget *button3, gpointer entry);
void go_homepage_4(GtkWidget *button4, gpointer entry);

//��صĶ�������������������д�߼�����
void homepage_1(int argc, char *argv[]);
void homepage_2(int argc, char *argv[]);
void homepage_3(int argc, char *argv[]);
void homepage_4(int argc, char *argv[]);


void go_homepage_1(GtkWidget *button1, gpointer entry){    //������ʾ����������ת

	homepage_1(2,entry);
}

void go_homepage_2(GtkWidget *button2, gpointer entry){		//���ڼ�����������ת

	homepage_2(2,entry);

}

void go_homepage_3(GtkWidget *button3, gpointer entry){

	homepage_3(2,entry);

}

void go_homepage_4(GtkWidget *button4, gpointer entry){

	homepage_4(2,entry);

}

void homepage_1(int argc, char *argv[]){showcalendar(argc, argv);}	//������������

void homepage_2(int argc, char *argv[]){calclient_thread(argc, argv);}//���ö��̼߳������ͻ��˺���	

void homepage_3(int argc, char *argv[]){calclient_process(argc, argv);}//���ö���̼������ͻ��˺���	

void homepage_4(int argc, char *argv[]){}



//���˵���ѡ���ܽ��棩

void homepage_select(int argc, char *argv[]){

	// ��ʼ��

	gtk_init(&argc, &argv);

	// �������㴰��

	window_select = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	// ���ô��ڵı���

	gtk_window_set_title(GTK_WINDOW(window_select), g_convert("��ҳ",-1,"UTF-8","GB2312",NULL,NULL,NULL));

	// ���ô��ڵ���С��С

	gtk_widget_set_size_request(window_select, 400, 600);
	
	// ���ô�������ʾ���е�λ��Ϊ����

	gtk_window_set_position(GTK_WINDOW(window_select), GTK_WIN_POS_CENTER);
	
	// �̶����ڵĴ�С

	gtk_window_set_resizable(GTK_WINDOW(window_select), FALSE);

	// "destroy" �� gtk_main_quit ����
	g_signal_connect(window_select, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	
	//����һ����ֱ��������
	vbox=gtk_vbox_new(FALSE,0);

	// �����������Ŵ�����
	gtk_container_add(GTK_CONTAINER(window_select), vbox);



	GtkWidget *button1 = gtk_button_new_with_label(g_convert("����",-1,"UTF-8","GB2312",NULL,NULL,NULL));
	gtk_box_pack_start(GTK_BOX(vbox),button1,TRUE,TRUE,40);
	
	GtkWidget *button2 = gtk_button_new_with_label(g_convert("���̼߳�����",-1,"UTF-8","GB2312",NULL,NULL,NULL));
	gtk_box_pack_start(GTK_BOX(vbox),button2,TRUE,TRUE,40);
	
	GtkWidget *button3 = gtk_button_new_with_label(g_convert("����̼�����",-1,"UTF-8","GB2312",NULL,NULL,NULL));
	gtk_box_pack_start(GTK_BOX(vbox),button3,TRUE,TRUE,40);
	
	GtkWidget *button4 = gtk_button_new_with_label(g_convert("����",-1,"UTF-8","GB2312",NULL,NULL,NULL));
	gtk_box_pack_start(GTK_BOX(vbox),button4,TRUE,TRUE,40);

	//�󶨵���¼�

	g_signal_connect(button1, "pressed", G_CALLBACK(go_homepage_1), NULL);

	g_signal_connect(button2, "pressed", G_CALLBACK(go_homepage_2), NULL);

	g_signal_connect(button3, "pressed", G_CALLBACK(go_homepage_3), NULL);

	g_signal_connect(button4, "pressed", G_CALLBACK(go_homepage_4), NULL);


	// ���ô���ͼƬ
    change_background(window_select, 400, 600, "./img/bg.jpg");
	// ��ʾ����ȫ���ؼ�

	gtk_widget_show_all(window_select);

	//������ѭ��

	gtk_main();

}


//���ô��ڱ���ͼƬ�ĺ���
void change_background(GtkWidget *widget, int w, int h, const gchar *path)
{
        //1.�����ڿ��Ի�ͼ
        gtk_widget_set_app_paintable(widget, TRUE);
        gtk_widget_realize(widget);
 
	/* ���ı���ͼʱ��ͼƬ���ص� 
	* ��ʱҪ�ֶ���������ĺ������ô��ڻ�ͼ����ʧЧ�����������ػ����¼����� expose �¼����� 
	*/ 
        gtk_widget_queue_draw(widget);
        GdkPixbuf *src = gdk_pixbuf_new_from_file(path, NULL);
        GdkPixbuf *dst = gdk_pixbuf_scale_simple(src,w,h,GDK_INTERP_BILINEAR);
 
	/* ����pixmapͼ��;  
	* NULL������Ҫ�ɰ�;  
	* 123�� 0~255��͸������͸�� 
	*/
        GdkPixmap *pixmap = NULL;
        gdk_pixbuf_render_pixmap_and_mask(dst,&pixmap,NULL,128);
		
	// ͨ��pixmap��widget����һ�ű���ͼ�����һ����������Ϊ: FALSE
        gdk_window_set_back_pixmap(widget->window,pixmap,FALSE);
        
 
        g_object_unref(src);
        g_object_unref(dst);
        g_object_unref(pixmap);
 
        return;
}

int main(int argc, char *argv[]){

	homepage_select(argc, argv);

	return 0;

}
