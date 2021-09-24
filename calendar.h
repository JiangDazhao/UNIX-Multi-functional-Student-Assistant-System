
#include"header.h"
 
gint status ;
GtkWidget* window ;
GtkWidget* calendar ;
GtkWidget *vbox ,*hbox,*hbox1;
GtkWidget *frame ;
GtkWidget *separator ;
GtkWidget *vbox1 ,*vbox2 , *vbox3;
GtkWidget *check_button ,*label,*label1 ,*label2 ,*label3 ,*button;
const gchar *str1,*str2,*str3 ;
gchar str[100] ;	
gint year, month , day ;
 
void change(GtkWidget *calendar){
	gtk_calendar_display_options(GTK_CALENDAR(calendar) , status);
}
 
void call_show_heading(GtkWidget *widget ,GtkWidget *calendar)
{
	status = ( status ^ 1 ) ;
	change(calendar);
}
void call_show_day_names(GtkWidget *widget ,GtkWidget *calendar)
{
	status = ( status ^ 2 ) ;
	change(calendar);
}
void call_no_month_change(GtkWidget *widget ,GtkWidget *calendar)
{
	status = ( status ^ 4 ) ;
	change(calendar);
}
void call_show_week_numbers(GtkWidget *widget ,GtkWidget *calendar)
{
	status = ( status ^ 8 ) ;
	change(calendar);
}
void call_week_start_monday(GtkWidget *widget ,GtkWidget *calendar)
{
	status = ( status ^ 16 ) ;
	change(calendar);	
}
 
void create_window(){
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(window) , GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window) , 400 , 300); 
	gtk_window_set_title(GTK_WINDOW(window) , "Calendar");
	gtk_container_set_border_width(GTK_CONTAINER(window) , 20);	
	//gtk_window_set_resizable(GTK_WINDOW(window) , FALSE);
	g_signal_connect(G_OBJECT(window) , "destroy", 
                         G_CALLBACK(gtk_main_quit) , NULL);
}
 
void call_day_select(GtkWidget *widget ,GtkWidget *data){		
	str3 = gtk_label_get_text( GTK_LABEL(label2) );	
	gtk_label_set_text(GTK_LABEL(label3) , str3);
	str2 = gtk_label_get_text( GTK_LABEL(label1) );	
	gtk_label_set_text(GTK_LABEL(label2) , str2);
	gtk_calendar_get_date(GTK_CALENDAR(widget) ,&year, &month ,&day);
	sprintf(str,"day_selected : %d年%d月%d日",year,month+1,day);
	gtk_label_set_text(GTK_LABEL(label1) ,str);		
}
void call_month_changed(GtkWidget *widget ,GtkWidget *data){	
	str3 = gtk_label_get_text( GTK_LABEL(label2) );	
	gtk_label_set_text(GTK_LABEL(label3) , str3);
	str2 = gtk_label_get_text( GTK_LABEL(label1) );	
	gtk_label_set_text(GTK_LABEL(label2) , str2);
	gtk_calendar_get_date(GTK_CALENDAR(widget) ,&year, &month ,&day);
	sprintf(str,"month_changed : %d年%d月%d日",year,month+1,day);
	gtk_label_set_text(GTK_LABEL(label1) ,str);		
}
void call_day_selected_double_click(GtkWidget *widget ,GtkWidget *data){	
	str3 = gtk_label_get_text( GTK_LABEL(label2) );	
	gtk_label_set_text(GTK_LABEL(label3) , str3);
	str2 = gtk_label_get_text( GTK_LABEL(label1) );	
	gtk_label_set_text(GTK_LABEL(label2) , str2);
	gtk_calendar_get_date(GTK_CALENDAR(widget) ,&year, &month ,&day);
	sprintf(str,"day_selected_double_click : %d年%d月%d日",year,month+1,day);
	gtk_label_set_text(GTK_LABEL(label1) ,str);		
}
void call_prev_month(GtkWidget *widget ,GtkWidget *data){	
	str3 = gtk_label_get_text( GTK_LABEL(label2) );	
	gtk_label_set_text(GTK_LABEL(label3) , str3);
	str2 = gtk_label_get_text( GTK_LABEL(label1) );	
	gtk_label_set_text(GTK_LABEL(label2) , str2);
	gtk_calendar_get_date(GTK_CALENDAR(widget) ,&year, &month ,&day);
	sprintf(str,"prev_month : %d年%d月%d日",year,month+1,day);
	gtk_label_set_text(GTK_LABEL(label1) ,str);		
}
void call_next_month(GtkWidget *widget ,GtkWidget *data){	
	str3 = gtk_label_get_text( GTK_LABEL(label2) );	
	gtk_label_set_text(GTK_LABEL(label3) , str3);
	str2 = gtk_label_get_text( GTK_LABEL(label1) );	
	gtk_label_set_text(GTK_LABEL(label2) , str2);
	gtk_calendar_get_date(GTK_CALENDAR(widget) ,&year, &month+1 ,&day);
	sprintf(str,"next_month : %d年%d月%d日",year,month,day);
	gtk_label_set_text(GTK_LABEL(label1) ,str);		
}
void call_prev_year(GtkWidget *widget ,GtkWidget *data){	
	str3 = gtk_label_get_text( GTK_LABEL(label2) );	
	gtk_label_set_text(GTK_LABEL(label3) , str3);
	str2 = gtk_label_get_text( GTK_LABEL(label1) );	
	gtk_label_set_text(GTK_LABEL(label2) , str2);
	gtk_calendar_get_date(GTK_CALENDAR(widget) ,&year, &month+1 ,&day);
	sprintf(str,"prev_year : %d年%d月%d日",year,month,day);
	gtk_label_set_text(GTK_LABEL(label1) ,str);		
}
void call_next_year(GtkWidget *widget ,GtkWidget *data){	
	str3 = gtk_label_get_text( GTK_LABEL(label2) );	
	gtk_label_set_text(GTK_LABEL(label3) , str3);
	str2 = gtk_label_get_text( GTK_LABEL(label1) );	
	gtk_label_set_text(GTK_LABEL(label2) , str2);
	gtk_calendar_get_date(GTK_CALENDAR(widget) ,&year, &month ,&day);
	sprintf(str,"next_year : %d年%d月%d日",year,month+1,day);
	gtk_label_set_text(GTK_LABEL(label1) ,str);		
}
 
void create_calendar(){
 
	frame = gtk_frame_new("Calendar") ;
	gtk_box_pack_start(GTK_BOX(hbox) , frame ,FALSE ,FALSE , 0 ); 
	gtk_widget_show(frame);
	calendar = gtk_calendar_new() ;
	gtk_calendar_display_options(GTK_CALENDAR(calendar) , 0 );
	gtk_container_add(GTK_CONTAINER(frame) , calendar);
	gtk_widget_show(calendar);
	g_signal_connect(G_OBJECT(calendar) ,"day_selected" ,G_CALLBACK(call_day_select),NULL);
	g_signal_connect(G_OBJECT(calendar) , "month_changed",G_CALLBACK(call_month_changed),NULL);
	g_signal_connect(G_OBJECT(calendar) , "day_selected_double_click",G_CALLBACK(call_day_selected_double_click),NULL);
	g_signal_connect(G_OBJECT(calendar) ,"prev_month" , G_CALLBACK(call_prev_month) , NULL);
	g_signal_connect(G_OBJECT(calendar) , "next_month" , G_CALLBACK(call_next_month) ,NULL);
	g_signal_connect(G_OBJECT(calendar) , "prev_year" , G_CALLBACK(call_prev_month),NULL);
	g_signal_connect(G_OBJECT(calendar) , "next_year" ,G_CALLBACK(call_next_year),NULL);
	separator = gtk_vseparator_new();
	gtk_box_pack_start(GTK_BOX(hbox) , separator , FALSE, FALSE , 0);
	gtk_widget_show(separator); 
}

void create_flags(){
	vbox3 = gtk_vbox_new(FALSE ,0);
	gtk_box_pack_start(GTK_BOX(hbox) , vbox3 ,FALSE, FALSE ,0);
	gtk_widget_show(vbox3);
	
	frame = gtk_frame_new("Flags") ;
	gtk_box_pack_start(GTK_BOX(vbox3) , frame, FALSE ,FALSE , 0);
	gtk_widget_show(frame);	
		
	vbox1 = gtk_vbox_new(FALSE , 0); 
	gtk_container_add(GTK_CONTAINER(frame) , vbox1); 
	gtk_widget_show(vbox1) ;
	
	check_button = gtk_check_button_new_with_label("Show Heading") ;
	gtk_box_pack_start(GTK_BOX(vbox1) , check_button ,FALSE , FALSE , 0 ) ;
	gtk_widget_show(check_button);
	g_signal_connect(G_OBJECT(check_button) , "clicked" ,
			G_CALLBACK(call_show_heading) , calendar );	
	
	check_button = gtk_check_button_new_with_label("Show Day Names") ;
	gtk_box_pack_start(GTK_BOX(vbox1) , check_button ,FALSE , FALSE , 0 ) ;
	gtk_widget_show(check_button);
	g_signal_connect(G_OBJECT(check_button) , "clicked" ,
			G_CALLBACK(call_show_day_names) , calendar );
 
	check_button = gtk_check_button_new_with_label("No Month Change") ;
	gtk_box_pack_start(GTK_BOX(vbox1) , check_button ,FALSE , FALSE , 0 ) ;
	gtk_widget_show(check_button);
	g_signal_connect(G_OBJECT(check_button) , "clicked" ,
			G_CALLBACK(call_no_month_change) , calendar );
 
	check_button = gtk_check_button_new_with_label("Show Week Numbers") ;
	gtk_box_pack_start(GTK_BOX(vbox1) , check_button ,FALSE , FALSE , 0 ) ;
	gtk_widget_show(check_button);
	g_signal_connect(G_OBJECT(check_button) , "clicked" ,
			G_CALLBACK(call_show_week_numbers) , calendar );
}

 

void showcalendar(int argc, char *argv[])
{	 
	status = 0 ;
	gtk_init(&argc,&argv);
	create_window() ;
	vbox = gtk_vbox_new(FALSE , 0 );
	gtk_container_add(GTK_CONTAINER(window) , vbox);
	gtk_widget_show(vbox);
	
	hbox = gtk_hbox_new(FALSE , 20 );
	gtk_container_add(GTK_CONTAINER(vbox) , hbox);
	gtk_widget_show(hbox) ;
	
	create_calendar() ;
	create_flags();	
	gtk_widget_show(window);
	gtk_main() ;	

}