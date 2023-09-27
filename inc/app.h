#ifndef __APP_H__
#define __APP_H__
#include <gtk/gtk.h>
#include "appflags.h"

#define BUILDER_MAIN			"ui/main1.glade"
#define BUILDER_ALARM			"ui/alarm_nomcu.glade"

struct AppGtkMainVars_s {
	GtkApplication *main_app;
	GtkWindow *mainWin, *alarmMCU;
	GtkStack *stackMain;
	GtkPopover *pad;
	GtkButton *sysBtn1, *sysBtn2, *sysBtn3;
};

#ifdef __MAIN_C__
struct AppGtkMainVars_s appGtkVars;
#else
extern struct AppGtkMainVars_s appGtkVars;
#endif

void app_exit(int sig);
void lbl_printf(GtkLabel *lbl, gchar *fmt, ...);
void ent_printf(GtkEntry *ent, gchar *fmt, ...);
#endif
