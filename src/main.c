#define _GNU_SOURCE
#include <stdio.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include "debug.h"
#include "appVars.h"
#include "app.h"
#include "g_utils.h"
#include "settings.h"
#include "sownorma.h"
#include "hw_keys.h"
#include "pad.h"
#include "mcu_connect.h"
#include "dlg_bunker.h"
#include "normaseed.h"

int test_cpp(void);

#ifdef __arm__
extern guchar _binary_main_glade_start;
extern guchar _binary_main_glade_end;
#endif

static GtkBuilder *bld = NULL;
static int mainSW = 0;
static void realize_cb(GtkWidget *w, gpointer p) {
#ifdef __arm__
//	gdk_window_set_cursor(gtk_widget_get_window(w), gdk_cursor_new_from_name(gdk_display_get_default(),"none"));
#endif
}

void hw_keys_up_cb(GtkWidget *w, guint keys, gpointer p) {
	if (keys)
		debugf("keyUp:%u\n", keys);
}

void hw_keys_down_cb(GtkWidget *w, guint keys, gpointer p) {
	if (keys)
		debugf("keyDown:%u\n", keys);
	mainSW++;
	if (mainSW > 1) mainSW = 0;
	if (mainSW) {
		gtk_stack_set_visible_child(appGtkVars.stackMain, (GtkWidget*)sowNorma.main);
	} else {
		gtk_stack_set_visible_child(appGtkVars.stackMain, (GtkWidget*)normaSeed.main);
	}
}

void app_exit(int sig) {

	g_application_quit((GApplication*)appGtkVars.main_app);
	struct PipesCMD_s cmd = {.cmd = pcmdExit, .len = 0};
	pipes_send(appVars.mcuPipe.pipe[0].out, (void*)&cmd, sizeof(cmd));
}

void error_no_mcu_signal() {
	guint w,h;
	gtk_window_get_size(appGtkVars.mainWin, &w, &h);
	gtk_widget_show((GtkWidget*)appGtkVars.alarmMCU);
//	gtk_window_set_keep_above(appGtkVars.alarmMCU, TRUE);
//	gtk_window_present(appGtkVars.alarmMCU);
	
	debugf("W: %d; H: %d\n", w,h);
}

void popup_realize_cb(GtkWidget *w, gpointer p) {
	gtk_window_move(w, 10,10);
}

void popup_map_cb(GtkWidget *w, gpointer p) {
	gtk_window_move(w, 10,10);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
static void create_pipes(struct PipePair_s *pp) {
	int fds[2];

	pipe2(fds, O_NONBLOCK);
	pp->pipe[0].in = fds[0];
	pp->pipe[1].out = fds[1];
	pipe2(fds, O_NONBLOCK);
	pp->pipe[0].out = fds[1];
	pp->pipe[1].in = fds[0];
}
#pragma GCC diagnostic pop

static void close_pipes(struct PipePair_s *pp) {
	close(pp->pipe[0].in);
	close(pp->pipe[1].out);
	close(pp->pipe[1].in);
	close(pp->pipe[0].out);
}

static void sys_btn_click(GtkButton *btn, gpointer p) {
	switch(*((char*)p)) {
		case 1:
			gtk_stack_set_visible_child(appGtkVars.stackMain, (GtkWidget*)normaSeed.main);
			break;
		case 2:
			gtk_stack_set_visible_child(appGtkVars.stackMain, (GtkWidget*)sowNorma.main);
			break;
		case 3:
			gtk_stack_set_visible_child(appGtkVars.stackMain, (GtkWidget*)dlgBunker.main);
			error_no_mcu_signal();
			break;
	}
	return;
}

static gboolean mcu_send_request(gpointer p) {
	mcu_send_commands();
	return G_SOURCE_CONTINUE;
}

static void app_activate(GtkApplication *app, gpointer p) {

	appVars.mcuThread = NULL;

	read_options("agro22.xml");

/*
#ifdef __arm__
 	{
		gssize blen = &_binary_main_glade_end - &_binary_main_glade_start;
		bld = gtk_builder_new_from_string(&_binary_main_glade_start, blen);
	}
#else
	bld = gtk_builder_new_from_file(BUILDER_MAIN);
#endif
*/
	{
		GtkBuilder *b = gtk_builder_new_from_file(BUILDER_ALARM);
		appGtkVars.alarmMCU = (GtkWindow*)gtk_builder_get_object(b, "alarm_mcu_win");
		g_signal_connect(appGtkVars.alarmMCU, "realize", G_CALLBACK(popup_realize_cb), NULL);
		g_signal_connect(appGtkVars.alarmMCU, "map", G_CALLBACK(popup_map_cb), NULL);
	//	g_object_unref(b);
	}
	bld = gtk_builder_new_from_file(BUILDER_MAIN);
	if (!bld) {
		puts("No resource file !");
		return;
	}
	gtk_builder_set_application(bld, app);

	appGtkVars.mainWin = (GtkWindow*)gtk_builder_get_object(bld, "mainWin");
	appGtkVars.stackMain = (GtkStack*)gtk_builder_get_object(bld, "stack_appMain");

	// System Buttons временные
	appGtkVars.sysBtn1 = (GtkButton*)gtk_builder_get_object(bld, "sys_btn1");
	appGtkVars.sysBtn2 = (GtkButton*)gtk_builder_get_object(bld, "sys_btn2");
	appGtkVars.sysBtn3 = (GtkButton*)gtk_builder_get_object(bld, "sys_btn3");

	g_signal_connect(appGtkVars.sysBtn1, "clicked", G_CALLBACK(sys_btn_click), "\x1");
	g_signal_connect(appGtkVars.sysBtn2, "clicked", G_CALLBACK(sys_btn_click), "\x2");
	g_signal_connect(appGtkVars.sysBtn3, "clicked", G_CALLBACK(sys_btn_click), "\x3");


	g_signal_connect(appGtkVars.mainWin, "realize", G_CALLBACK(realize_cb), NULL);

	gtk_application_add_window(app, appGtkVars.mainWin);

	{
//		gtk_window_fullscreen(appGtkVars.mainWin);
		gtk_window_set_default_size(appGtkVars.mainWin, 1280, 800);
	}

	norma_seed_init();
	sownorma_option_init();
	appGtkVars.pad = pad_init();
	create_pipes(&appVars.mcuPipe);
	GIOChannel *mcuIn = g_io_channel_unix_new(appVars.mcuPipe.pipe[0].in);
	g_io_channel_set_encoding(mcuIn, NULL, NULL);
	g_io_add_watch(mcuIn, G_IO_IN, mcu_read_pipe, NULL);
	
/*
	if (appSettings.PortContr) {
		struct MCUConnect_s mcu;
		mcu.dev = appSettings.PortContr;
		mcu.baud = appSettings.PortSpeed;
		mcu.pIn = appVars.mcuPipe.pipe[1].in;
		mcu.pOut = appVars.mcuPipe.pipe[1].out;
		mcu.status = MCU_Ok;
		gboolean st = mcu_serial_init(&mcu);
		if (st)
			mcu_serial_thread(NULL);
		else
			debugf("Error port:%hd\n", mcu.status);
	}
*/
	if (appSettings.PortControl.dev) {
		struct MCUConnect_s *mcu = malloc(sizeof(struct MCUConnect_s));
		mcu->dev = appSettings.PortControl.dev;
		mcu->baud = appSettings.PortControl.speed;
		mcu->pIn = appVars.mcuPipe.pipe[1].in;
		mcu->pOut = appVars.mcuPipe.pipe[1].out;
		mcu->status = MCU_Ok;
		gboolean st = mcu_serial_init(mcu);
		if (st) {
			appVars.mcuThread = g_thread_new("MCUSerial", mcu_serial_thread, mcu);
			g_timeout_add_seconds(1, mcu_send_request, NULL);
		} else {
			debugf("Error port:%hd\n", mcu->status);
			free(mcu);
		}
	}

//	init_ogl(bld);
/*
#ifdef __arm__
	GdkRectangle rec;
	gdk_monitor_get_workarea(gdk_display_get_primary_monitor(gdk_display_get_default()), &rec);
	gtk_window_set_default_size(appGtkVars.mainWin, rec.width, rec.height);
#endif
*/

	dlg_bunker_init();
	hw_keys_init("/dev/gpiochip0", appGtkVars.mainWin);
#ifdef __arm__
	g_signal_connect(appGtkVars.mainWin, "hw_keys_up", G_CALLBACK(hw_keys_up_cb), NULL);
	g_signal_connect(appGtkVars.mainWin, "hw_keys_down", G_CALLBACK(hw_keys_down_cb), NULL);
#endif
	pad_add_focus_events((GtkWidget*)normaSeed.entKulturaName);
	pad_add_focus_events((GtkWidget*)normaSeed.entSemyanGa);
//	gtk_window_set_default_size(appGtkVars.mainWin, 1280, 800);
	gtk_widget_show_all((GtkWidget*)appGtkVars.mainWin);
	signal(SIGINT, app_exit);
	signal(SIGTERM, app_exit);
}
static void app_shutdown(GtkApplication *app, gpointer p) {
	if (bld) {
		g_object_unref(bld);
	}
}

int main(int argc, char **argv) {
	int status = 0;
#ifdef __arm__
	appGtkVars.main_app = gtk_application_new("com.svahard.ogles2probe1", G_APPLICATION_FLAGS_NONE);
#else
	appGtkVars.main_app = gtk_application_new("com.svahard.ogles2probe1", G_APPLICATION_DEFAULT_FLAGS);
#endif
	test_cpp();
	g_signal_connect(appGtkVars.main_app, "activate", G_CALLBACK(app_activate), NULL);
	g_signal_connect(appGtkVars.main_app, "shutdown", G_CALLBACK(app_shutdown), NULL);
	status = g_application_run((GApplication*)appGtkVars.main_app, argc, argv);
	if (appVars.mcuThread) 
		g_thread_join(appVars.mcuThread);
	g_object_unref(appGtkVars.main_app);
	puts("Goodbye !");
	return(status);
}


