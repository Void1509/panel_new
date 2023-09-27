#include <stdio.h>
#include <gtk/gtk.h>
#include "debug.h"
#include "app.h"

/*
extern guchar _binary_pad_glade_start;
extern guchar _binary_pad_glade_end;

extern guchar _binary_pad_css_start;
extern guchar _binary_pad_css_end;
*/
extern const char *icons_xpm[];

static GtkPopover *keypad;
static struct {
	GtkButton *btd[10];
	GtkCssProvider *btd_css;
	GtkButton *hide, *enter, *erase;
	GtkGrid *pad_grid;
}pad;

static GdkPixbuf *icons, *ico_ok, *ico_set, *ico_erase;
static GtkWidget *oldWidget = NULL;
static int hide = 0;
static guint btn_num[]={0,1,2,3,4,5,6,7,8,9};
/*
static gboolean load_pad_css_provider() {
	gssize clen = &_binary_pad_css_end - &_binary_pad_css_start;
	GtkCssProvider *prov = gtk_css_provider_new();
	if (gtk_css_provider_load_from_data(prov, &_binary_pad_css_start,clen, NULL)) {
//		gtk_style_context_add_provider(gtk_widget_get_style_context((GtkWidget*)keypad), (GtkStyleProvider*)prov, GTK_STYLE_PROVIDER_PRIORITY_USER);
		pad.btd_css = prov;
		return TRUE;
	}
	return FALSE;
}
*/
static void btn_cb(GtkButton *bt, gpointer p) {
	guint *key = p;
	gchar str[10];
	printf("Key press:%d\n",*key);
	sprintf(str,"%d",*key);
	GtkEntry *ent = (GtkEntry*) gtk_popover_get_relative_to(keypad);
	g_signal_emit_by_name(ent, "insert-at-cursor", str, NULL);
}
static void erase_cb(GtkButton *bt, gpointer p) {

	GtkEntry *ent = (GtkEntry*) gtk_popover_get_relative_to(keypad);
//	g_signal_emit_by_name(ent, "delete-from-cursor", GTK_DELETE_CHARS, 1, NULL);
	g_signal_emit_by_name(ent, "backspace",  NULL);
}
static void hide_cb(GtkButton *bt, gpointer p) {

	gtk_widget_hide((GtkWidget*)keypad);
	gtk_popover_popdown(keypad);
	hide = 1;
}

void ent_ins_num(GtkEditable *ed, gchar *str, gint len, gpointer pos, gpointer p) {
	while(len--) {
		if ((*str > 0x39) || (*str < 0x30)) *str = 0x30;
		str++;
	}
}

gboolean ent_enter_num(GtkWidget *ent, GdkEvent *ev, gpointer p) {
	puts("Enter ent");
	if (ent != oldWidget)
		hide = 0;
	if (!hide) {
		gtk_popover_set_relative_to(keypad, ent);
		gtk_popover_popup(keypad);
		oldWidget = ent;
	}
	return FALSE;
}

gboolean ent_live_num(GtkWidget *ent, GdkEvent *ev, gpointer p) {
	gint64 i = g_ascii_strtoll(gtk_entry_get_text((GtkEntry*)ent), NULL, 0);
	if (i == 0) gtk_entry_set_text((GtkEntry*)ent, "0");
	puts("Live ent");
	return FALSE;
}

void pad_add_focus_events(GtkWidget *w) {
	gtk_widget_add_events(w, GDK_FOCUS_CHANGE_MASK);
	g_signal_connect(w, "insert-text", G_CALLBACK(ent_ins_num), NULL);
	g_signal_connect(w, "focus-out-event", G_CALLBACK(ent_live_num), NULL);
	g_signal_connect(w, "focus-in-event", G_CALLBACK(ent_enter_num), NULL);
}

GtkPopover *pad_init() {
//	gssize blen = &_binary_pad_glade_end - &_binary_pad_glade_start;
//	printf("size:%u\n", blen);
	keypad = NULL;
	GtkBuilder *bld = gtk_builder_new_from_file("ui/pad.glade");
//	GtkBuilder *bld = gtk_builder_new_from_string(&_binary_pad_glade_start, blen);
	if (!bld) return(keypad);

	icons = gdk_pixbuf_new_from_xpm_data(icons_xpm);
	ico_set = gdk_pixbuf_new_subpixbuf(icons, 0,0, 64,64);
	ico_ok = gdk_pixbuf_new_subpixbuf(icons, 64,0, 64,64);
	ico_erase = gdk_pixbuf_new_subpixbuf(icons, 128,0, 64,64);
	keypad = (GtkPopover*)gtk_builder_get_object(bld, "padWin");
	pad.pad_grid = (GtkGrid*)gtk_builder_get_object(bld,"pad_grid");
//	load_pad_css_provider();
	pad.btd[0] = (GtkButton*)gtk_builder_get_object(bld, "btn_key13");
	pad.btd[1] = (GtkButton*)gtk_builder_get_object(bld, "btn_key8");
	pad.btd[2] = (GtkButton*)gtk_builder_get_object(bld, "btn_key9");
	pad.btd[3] = (GtkButton*)gtk_builder_get_object(bld, "btn_key10");
	pad.btd[4] = (GtkButton*)gtk_builder_get_object(bld, "btn_key4");
	pad.btd[5] = (GtkButton*)gtk_builder_get_object(bld, "btn_key5");
	pad.btd[6] = (GtkButton*)gtk_builder_get_object(bld, "btn_key6");
	pad.btd[7] = (GtkButton*)gtk_builder_get_object(bld, "btn_key0");
	pad.btd[8] = (GtkButton*)gtk_builder_get_object(bld, "btn_key1");
	pad.btd[9] = (GtkButton*)gtk_builder_get_object(bld, "btn_key2");
	for (guint ui1 = 0; ui1 < 10; ui1++) {
		g_signal_connect(pad.btd[ui1], "clicked", G_CALLBACK(btn_cb), &btn_num[ui1]);
//		gtk_style_context_add_provider(gtk_widget_get_style_context((GtkWidget*)pad.btd[ui1]), (GtkStyleProvider*)pad.btd_css, GTK_STYLE_PROVIDER_PRIORITY_USER);
	}
	pad.hide = (GtkButton*)gtk_builder_get_object(bld,"btn_key12");
	pad.enter = (GtkButton*)gtk_builder_get_object(bld,"btn_key15");
	pad.erase = (GtkButton*)gtk_builder_get_object(bld,"btn_key3");
	gtk_button_set_image(pad.hide, gtk_image_new_from_pixbuf(ico_set));
	gtk_button_set_image(pad.enter, gtk_image_new_from_pixbuf(ico_ok));
	gtk_button_set_image(pad.erase, gtk_image_new_from_pixbuf(ico_erase));
	g_signal_connect(pad.hide, "clicked", G_CALLBACK(hide_cb), NULL);
	g_signal_connect(pad.erase, "clicked", G_CALLBACK(erase_cb), NULL);
	if (!keypad) debug("Keypad false !\n");
	return(keypad);
}

