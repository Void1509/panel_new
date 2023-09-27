#include "app.h"
#include "cairo.h"
#include "mcu_data.h"
#include "debug.h"
#include "dlg_bunker.h"

#define GEARS_MAX		6
#define FANS_MAX		3

static struct {
	GtkDrawingArea *draw;
}bunkers[2];

struct Gears_s{
	uint8_t inx;
	uint8_t anim;
	GtkDrawingArea *draw;
};

struct Fans_s {
	int8_t inx;
	uint8_t anim;
	int8_t rev;
	GtkDrawingArea *draw;
};

static struct Plug_s {
	int8_t inx;
	uint8_t anim;
	unsigned up:1;
	unsigned upd:1;		// updated
}plug;

GtkWidget *towers_init();

static struct Gears_s gears[3];
static struct Fans_s fans[3];
static GtkWidget *drawPlug, *tow;

static char test_err(GError **err) {
	if (*err) {
		debugf("Error :%s\n", (*err)->message);
		g_error_free(*err);
		*err = NULL;
	}
}

static char resources_init(char *rpath) {
	char ret = 0;
	GError *err = NULL;
	GResource *res = NULL;

	res = g_resource_load(rpath, &err);

	if (err) {
		debugf("Error load resource: %s\n",err->message);
		g_error_free(err);
	} else {
		g_resources_register(res);
		dlgBunker.bunkers = gdk_pixbuf_new_from_resource("/bunker/form0/bunkers.png", &err);
		dlgBunker.gears = gdk_pixbuf_new_from_resource("/bunker/form0/gears.png", &err);
		dlgBunker.gearsblue = gdk_pixbuf_new_from_resource("/bunker/form0/gearsblue.png", &err);
		dlgBunker.gearsred = gdk_pixbuf_new_from_resource("/bunker/form0/gearsred.png", &err);
		dlgBunker.fansl = gdk_pixbuf_new_from_resource("/bunker/form0/fansl.png", &err);
		dlgBunker.fansr = gdk_pixbuf_new_from_resource("/bunker/form0/fansr.png", &err);
		dlgBunker.plug = gdk_pixbuf_new_from_resource("/bunker/form0/plug.png", &err);
		dlgBunker.plug1 = gdk_pixbuf_new_from_resource("/bunker/form0/plug1.png", &err);
		dlgBunker.grunt = gdk_pixbuf_new_from_resource("/bunker/form0/grunt.png", &err);
		test_err(&err);
	}
	g_resource_unref(res);
	return ret;
}

void bunkers_update(uint8_t inx) {
	if (inx > 1) return;

//	debugf("CMD61 inx:%hhd data:%4hx\n", inx, mcuData.bunkers);
	gtk_widget_queue_draw((GtkWidget*)bunkers[inx].draw);
//	mcuData.bunkers	;
}

void shaft_update(uint8_t inx) {
//	debugf("Shaft:%4X\n", mcuData.shaft[inx]);
	if (mcuData.shaft[inx] == 0xffff) {
		gears[inx].anim = 0;
	} else {
		gears[inx].anim = 1;
	}
}

void fans_update(uint8_t inx) {
//	debugf("Fans:%4hX\n", mcuData.fans[inx]);
	if (mcuData.fans[inx] == 0xffff) {
		fans[inx].anim = 0;
	} else {
		fans[inx].anim = 1;
	}
}

void plug_update(uint8_t inx) {
	if (mcuData.dsens != plug.up) {
		plug.up = mcuData.dsens;
		plug.anim = 1;
		gtk_widget_queue_draw(drawPlug);
	}
}

static gboolean time_func(gpointer p) {
	for(int i = 0; i < 3; i++) {
		if (gears[i].draw) {
			if (gears[i].anim) 
				gtk_widget_queue_draw((GtkWidget*)gears[i].draw);
		}
		if (fans[i].draw) {
			if (fans[i].anim) 
				gtk_widget_queue_draw((GtkWidget*)fans[i].draw);
		}
	}
	if (plug.anim) {
		gtk_widget_queue_draw((GtkWidget*)drawPlug);
	}
	return TRUE;
}

static void dlgBunker_map(GtkWidget *w, gpointer p) {
	struct DlgBunker_s *dlg = p;
	dlg->toId = g_timeout_add(50, time_func, NULL);
	debug("Map draw\n");
}

static void dlgBunker_unmap(GtkWidget *w, gpointer p) {
	struct DlgBunker_s *dlg = p;
	g_source_remove(dlg->toId);
	debug("UnMap draw\n");
}

static void draw_fan(cairo_t *ctx, GdkPixbuf *pb, int inx, int x, int y) {
	GdkPixbuf *ico = gdk_pixbuf_new_subpixbuf(pb, (inx * 88), 0, 88,101);
	gdk_cairo_set_source_pixbuf(ctx, ico, x, y);
	cairo_paint(ctx);
	g_object_unref(ico);
}

static void draw_gears(cairo_t *ctx, int inx, int x, int y) {
	GdkPixbuf *ico = gdk_pixbuf_new_subpixbuf(dlgBunker.gearsred, (inx * 91), 0, 91,91);
	gdk_cairo_set_source_pixbuf(ctx, ico, x, y);
	cairo_paint(ctx);
	g_object_unref(ico);
}

static gboolean drawPlug_cb(GtkWidget *w, cairo_t *ctx, gpointer p) {
	gdk_cairo_set_source_pixbuf(ctx, dlgBunker.grunt, 0, 145);
	cairo_paint(ctx);
	if (mcuData.dsens == 0xff) return TRUE;

	if (plug.anim) {
		if (mcuData.dsens) {
			if (plug.inx < 5)
				plug.inx++;
			else 
				plug.anim = 0;
		} else {
			if (plug.inx > 0)
				plug.inx--;
			else 
				plug.anim = 0;
		}

		gdk_cairo_set_source_pixbuf(ctx, dlgBunker.plug1, 0, plug.inx * 10);
		cairo_paint(ctx);
	} else {
		if (!mcuData.dsens) {
			gdk_cairo_set_source_pixbuf(ctx, dlgBunker.plug1, 0, 0);
		} else {
			gdk_cairo_set_source_pixbuf(ctx, dlgBunker.plug1, 0, 50);
		}

		cairo_paint(ctx);
	}
/*
	uint8_t inx = 0;
	if (mcuData.dsens == 1) inx = 1;

	GdkPixbuf *ico = gdk_pixbuf_new_subpixbuf(dlgBunker.plug, (inx * 144), 0, 144,224);
	gdk_cairo_set_source_pixbuf(ctx, ico, 0, 0);
	cairo_paint(ctx);
	g_object_unref(ico);
*/
}

static gboolean draw_fans(GtkWidget *w, cairo_t *ctx, gpointer p) {
	uint8_t inx = *((uint8_t*)p) - 48;

	if (fans[inx].rev >= 0)
		draw_fan(ctx, dlgBunker.fansr, fans[inx].inx, 0, 0);
	else
		draw_fan(ctx, dlgBunker.fansl, fans[inx].inx, 0, 0);

	if (fans[inx].anim) {
		fans[inx].inx += fans[inx].rev;
		if ((fans[inx].inx == FANS_MAX) && (fans[inx].rev >= 0)) fans[inx].inx = 0;
		if ((fans[inx].inx < 0) && (fans[inx].rev < 0)) fans[inx].inx = FANS_MAX - 1;
	}
	return TRUE;
}

static gboolean draw_shaft(GtkWidget *w, cairo_t *ctx, gpointer p) {
	uint8_t inx = *((uint8_t*)p) - 48;

	draw_gears(ctx, gears[inx].inx, 0, 0);
	if (gears[inx].anim) {
		gears[inx].inx++;
		if (gears[inx].inx == GEARS_MAX) gears[inx].inx = 0;
	}
	return TRUE;
}
static gboolean draw_bunker(GtkWidget *w, cairo_t *ctx, gpointer p) {
	char *pp = p;
	uint8_t inx = -1;
	int ico = 0;

	inx = (*pp - 48) & 3;
	if (inx > 1) return TRUE;

	if (mcuData.bunkers[inx] == 0xff) ico = 0;
	if (mcuData.bunkers[inx] == 0x00) ico = 1;
	if (mcuData.bunkers[inx] == 0x01) ico = 2;
	if (mcuData.bunkers[inx] == 0x11) ico = 3;
	GdkPixbuf *icon = gdk_pixbuf_new_subpixbuf(dlgBunker.bunkers, (ico * 130), 0, 130, 198);
	gdk_cairo_set_source_pixbuf(ctx, icon, 0,0);
	cairo_paint(ctx);
	g_object_unref(icon);
	return TRUE;
}
void dlg_bunker_init() {
	GtkBuilder *bld = gtk_builder_new_from_file("ui/dlg_bunker.glade");

	fans[0].rev = 1;
	fans[1].rev = -1;
	fans[2].rev = 1;
	if (bld) {
		resources_init("build/appres.gresource");
		dlgBunker.main = (GtkBox*)gtk_builder_get_object(bld, "dlg_bunker");
		bunkers[0].draw = (GtkDrawingArea*)gtk_builder_get_object(bld, "draw_bunker1");
		bunkers[1].draw = (GtkDrawingArea*)gtk_builder_get_object(bld, "draw_bunker2");
		gears[0].draw = (GtkDrawingArea*)gtk_builder_get_object(bld, "draw_shaft1");
		gears[1].draw = (GtkDrawingArea*)gtk_builder_get_object(bld, "draw_shaft2");
		gears[2].draw = NULL;
//		gears[0].anim = 1;
//		gears[1].anim = 1;
		fans[0].draw = (GtkDrawingArea*)gtk_builder_get_object(bld, "draw_fan1");
		fans[1].draw = (GtkDrawingArea*)gtk_builder_get_object(bld, "draw_fan2");
		fans[2].draw = (GtkDrawingArea*)gtk_builder_get_object(bld, "draw_fan3");

		drawPlug = (GtkWidget*)gtk_builder_get_object(bld,"draw_plug");
		if (dlgBunker.main) {
//			gtk_container_add((GtkContainer*)dlgBunker.main, towers_init());
			gtk_box_pack_start(dlgBunker.main, towers_init(), TRUE, TRUE, 1);
//			tow = towers_init();
//			gtk_container_add(dlgBunker.main, tow);
			gtk_widget_show((GtkWidget*)dlgBunker.main);
			gtk_stack_add_named(appGtkVars.stackMain, GTK_WIDGET(dlgBunker.main), "DlgBunker");

			g_signal_connect(bunkers[0].draw, "draw", G_CALLBACK(draw_bunker), "0");
			g_signal_connect(bunkers[1].draw, "draw", G_CALLBACK(draw_bunker), "1");
			g_signal_connect(gears[0].draw, "draw", G_CALLBACK(draw_shaft), "0");
			g_signal_connect(gears[1].draw, "draw", G_CALLBACK(draw_shaft), "1");
			g_signal_connect(fans[0].draw, "draw", G_CALLBACK(draw_fans), "0");
			g_signal_connect(fans[1].draw, "draw", G_CALLBACK(draw_fans), "1");
			g_signal_connect(fans[2].draw, "draw", G_CALLBACK(draw_fans), "2");
			g_signal_connect(drawPlug, "draw", G_CALLBACK(drawPlug_cb), NULL);



			g_signal_connect(dlgBunker.main, "map", G_CALLBACK(dlgBunker_map), &dlgBunker);
			g_signal_connect(dlgBunker.main, "unmap", G_CALLBACK(dlgBunker_unmap), &dlgBunker);



			gtk_stack_set_visible_child(appGtkVars.stackMain, GTK_WIDGET(dlgBunker.main));
		} else {
			debug("Error find dlg_bunker.ui\n");
		}
	}
}
