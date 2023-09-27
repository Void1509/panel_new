#ifndef __HW_KEYS_H__
#define __HW_KEYS_H__
#include <glib-2.0/glib.h>

#define HW_KEY_PG0			1
#define HW_KEY_PG1			2
#define HW_KEY_PG2			4
#define HW_KEY_PG3			8

gboolean hw_keys_init(gchar *devNamei, gpointer instance);

#endif
