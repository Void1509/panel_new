#include "app.h"
#include "debug.h"
#include "normaseed.h"

static void btn_appExit_clicked_cb(GtkButton *b, gpointer p) {
	app_exit(9);
}

void norma_seed_init() {
	GtkBuilder *bld = gtk_builder_new_from_file("ui/norma.glade");

	if (bld) {
		normaSeed.main = (GtkBox*)gtk_builder_get_object(bld, "boxMainNorma");
		normaSeed.appExit = (GtkButton*)gtk_builder_get_object(bld, "btn_appExit");
		normaSeed.entKulturaName = (GtkEntry*)gtk_builder_get_object(bld, "entKulturaName");
		normaSeed.entSemyanGa = (GtkEntry*)gtk_builder_get_object(bld, "entSemyanGa");

		g_signal_connect(normaSeed.appExit, "clicked", G_CALLBACK(btn_appExit_clicked_cb), NULL);
		
		gtk_widget_show((GtkWidget*)normaSeed.main);
		gtk_stack_add_named(appGtkVars.stackMain, GTK_WIDGET(normaSeed.main), "NormaSeeding");
	}
}
