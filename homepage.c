#include "header.h"
#include "calendar.h"
#include "calculator_client_thread.h"
#include "calculator_client_process.h"

GtkWidget *window_select; //主菜单的选择功能界面

GtkWidget *vbox;	//垂直盒子容器
GtkWidget *hbox;	//水平盒子容器

//gtk特殊的机制，回调函数，可以用这个函数确定按钮绑定事件执行的动作，发出动作指令
void go_homepage_1(GtkWidget *button1, gpointer entry);
void go_homepage_2(GtkWidget *button2, gpointer entry);
void go_homepage_3(GtkWidget *button3, gpointer entry);
void go_homepage_4(GtkWidget *button4, gpointer entry);

//相关的动作函数声明，在里面写逻辑代码
void homepage_1(int argc, char *argv[]);
void homepage_2(int argc, char *argv[]);
void homepage_3(int argc, char *argv[]);
void homepage_4(int argc, char *argv[]);


void go_homepage_1(GtkWidget *button1, gpointer entry){    //用于显示日历功能跳转

	homepage_1(2,entry);
}

void go_homepage_2(GtkWidget *button2, gpointer entry){		//用于计算器功能跳转

	homepage_2(2,entry);

}

void go_homepage_3(GtkWidget *button3, gpointer entry){

	homepage_3(2,entry);

}

void go_homepage_4(GtkWidget *button4, gpointer entry){

	homepage_4(2,entry);

}

void homepage_1(int argc, char *argv[]){showcalendar(argc, argv);}	//调用日历函数

void homepage_2(int argc, char *argv[]){calclient_thread(argc, argv);}//调用多线程计算器客户端函数	

void homepage_3(int argc, char *argv[]){calclient_process(argc, argv);}//调用多进程计算器客户端函数	

void homepage_4(int argc, char *argv[]){}



//主菜单（选择功能界面）

void homepage_select(int argc, char *argv[]){

	// 初始化

	gtk_init(&argc, &argv);

	// 创建顶层窗口

	window_select = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	// 设置窗口的标题

	gtk_window_set_title(GTK_WINDOW(window_select), g_convert("首页",-1,"UTF-8","GB2312",NULL,NULL,NULL));

	// 设置窗口的最小大小

	gtk_widget_set_size_request(window_select, 400, 600);
	
	// 设置窗口在显示器中的位置为居中

	gtk_window_set_position(GTK_WINDOW(window_select), GTK_WIN_POS_CENTER);
	
	// 固定窗口的大小

	gtk_window_set_resizable(GTK_WINDOW(window_select), FALSE);

	// "destroy" 和 gtk_main_quit 连接
	g_signal_connect(window_select, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	
	//创建一个垂直盒子容器
	vbox=gtk_vbox_new(FALSE,0);

	// 将布局容器放窗口中
	gtk_container_add(GTK_CONTAINER(window_select), vbox);



	GtkWidget *button1 = gtk_button_new_with_label(g_convert("日历",-1,"UTF-8","GB2312",NULL,NULL,NULL));
	gtk_box_pack_start(GTK_BOX(vbox),button1,TRUE,TRUE,40);
	
	GtkWidget *button2 = gtk_button_new_with_label(g_convert("多线程计算器",-1,"UTF-8","GB2312",NULL,NULL,NULL));
	gtk_box_pack_start(GTK_BOX(vbox),button2,TRUE,TRUE,40);
	
	GtkWidget *button3 = gtk_button_new_with_label(g_convert("多进程计算器",-1,"UTF-8","GB2312",NULL,NULL,NULL));
	gtk_box_pack_start(GTK_BOX(vbox),button3,TRUE,TRUE,40);
	
	GtkWidget *button4 = gtk_button_new_with_label(g_convert("聊天",-1,"UTF-8","GB2312",NULL,NULL,NULL));
	gtk_box_pack_start(GTK_BOX(vbox),button4,TRUE,TRUE,40);

	//绑定点击事件

	g_signal_connect(button1, "pressed", G_CALLBACK(go_homepage_1), NULL);

	g_signal_connect(button2, "pressed", G_CALLBACK(go_homepage_2), NULL);

	g_signal_connect(button3, "pressed", G_CALLBACK(go_homepage_3), NULL);

	g_signal_connect(button4, "pressed", G_CALLBACK(go_homepage_4), NULL);


	// 设置窗口图片
    change_background(window_select, 400, 600, "./img/bg.jpg");
	// 显示窗口全部控件

	gtk_widget_show_all(window_select);

	//启动主循环

	gtk_main();

}


//设置窗口背景图片的函数
void change_background(GtkWidget *widget, int w, int h, const gchar *path)
{
        //1.允许窗口可以绘图
        gtk_widget_set_app_paintable(widget, TRUE);
        gtk_widget_realize(widget);
 
	/* 更改背景图时，图片会重叠 
	* 这时要手动调用下面的函数，让窗口绘图区域失效，产生窗口重绘制事件（即 expose 事件）。 
	*/ 
        gtk_widget_queue_draw(widget);
        GdkPixbuf *src = gdk_pixbuf_new_from_file(path, NULL);
        GdkPixbuf *dst = gdk_pixbuf_scale_simple(src,w,h,GDK_INTERP_BILINEAR);
 
	/* 创建pixmap图像;  
	* NULL：不需要蒙版;  
	* 123： 0~255，透明到不透明 
	*/
        GdkPixmap *pixmap = NULL;
        gdk_pixbuf_render_pixmap_and_mask(dst,&pixmap,NULL,128);
		
	// 通过pixmap给widget设置一张背景图，最后一个参数必须为: FALSE
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
