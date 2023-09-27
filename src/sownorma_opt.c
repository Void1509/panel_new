#include "app.h"
#include "debug.h"
#include "sownorma.h"


void sownorma_option_init() {
	GtkBuilder *bld = gtk_builder_new_from_file("ui/dlg_sownorma_option.glade");

	if (bld) {
//		sowNorma.main = (GtkBox*)gtk_builder_get_object(bld, "box_SowNorma");
		sowNorma.main = (GtkBox*)gtk_builder_get_object(bld, "sowNorma");
		if (sowNorma.main) {
			gtk_widget_show((GtkWidget*)sowNorma.main);
			gtk_stack_add_named(appGtkVars.stackMain, GTK_WIDGET(sowNorma.main), "SowNormaOptions");
//			gtk_stack_set_visible_child(appGtkVars.stackMain, GTK_WIDGET(sowNorma.main));
		} else {
			debug("Error find sowNorma\n");
		}
	}
}
