#ifndef __DLG_BUNKER_H__
#define __DLG_BUNKER_H__
#include "app.h"

struct DlgBunker_s {
	GtkBox *main;
	GdkPixbuf *bunkers, *gears, *gearsblue, *gearsred;
	GdkPixbuf *fansl, *fansr, *plug;
	GdkPixbuf *plug1, *grunt;
	guint toId;			// timer ID
};

#ifdef __DLG_BUNKER_C__
struct DlgBunker_s dlgBunker;
#else
extern struct DlgBunker_s dlgBunker;
#endif

void dlg_bunker_init();

#endif
