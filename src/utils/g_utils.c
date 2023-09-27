#include <glib-object.h>
#include <gobject/gvaluecollector.h>
#include <unistd.h>
#include "g_utils.h"
// MainWindow table
/*
const struct Signals_s mainWinSignalsTable[] = \ 
{   {mainWin, "realize", realize_cb, NULL},
	{btn_switch, "toggled", btn_switch_toggled_cb, app},
	{btnExit, "clicked", app_exit, app}
}; 
*/

void signals_connect(struct Signals_s *table) {
	while(table->inst) {
		g_signal_connect(table->inst, table->sigName, table->func, table->data);
		table++;
	}
}

void pipes_channel(struct GIOPipes_s *ps, int in, int out) {
	ps->in = g_io_channel_unix_new(in);
	ps->out = g_io_channel_unix_new(out);
	g_io_channel_set_encoding(ps->in, NULL, NULL);
	g_io_channel_set_encoding(ps->out, NULL, NULL);
}
void pipes_send(int pd, const void *data, size_t len) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
	write(pd, data, len);
	fsync(pd);
#pragma GCC diagnostic pop
}
