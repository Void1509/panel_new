#include <stdint.h>
#include <glib-object.h>
#include <gobject/gvaluecollector.h>
#ifdef __arm__
#include <gpiod.h>
#endif
#include "debug.h"
#include "hw_keys.h"

struct HWKeySwitch_s {
	unsigned pg0:1;
	unsigned pg1:1;
	unsigned pg2:1;
	unsigned pg3:1;
};

static struct gpiod_chip *gpioChip = NULL;
static struct gpiod_line *pinPG0, *pinPG1, *pinPG2, *pinPG3;
static struct HWKeySwitch_s key, keyOld;

static gboolean get_key(gpointer instObj) {
#ifdef __arm__
	guint ukey = 0;
	guint dkey = 0;
	key.pg0 = gpiod_line_get_value(pinPG0);
	if (key.pg0 ^ keyOld.pg0) {
		keyOld.pg0 = key.pg0;
		if (!key.pg0) {
			puts("press putton");
			dkey |= HW_KEY_PG0;
		} else {
			ukey |= HW_KEY_PG0;
		}
	}

	key.pg1 = gpiod_line_get_value(pinPG1);
	if (key.pg1 ^ keyOld.pg1) {
		keyOld.pg1 = key.pg1;
		if (!key.pg1) {
			puts("press putton");
			dkey |= HW_KEY_PG1;
		} else {
			ukey |= HW_KEY_PG1;
		}
	}

	key.pg2 = gpiod_line_get_value(pinPG2);
	if (key.pg2 ^ keyOld.pg2) {
		keyOld.pg2 = key.pg2;
		if (!key.pg2) {
			puts("press putton");
			dkey |= HW_KEY_PG2;
		} else {
			ukey |= HW_KEY_PG2;
		}
	}

	key.pg3 = gpiod_line_get_value(pinPG3);
	if (key.pg3 ^ keyOld.pg3) {
		keyOld.pg3 = key.pg3;
		if (!key.pg3) {
			puts("press putton");
			dkey |= HW_KEY_PG3;
		} else {
			ukey |= HW_KEY_PG3;
		}
	}

	if (ukey) {
		g_signal_emit_by_name(instObj, "hw_keys_up", ukey);
		ukey = 0;
	}
	if (dkey) {
		g_signal_emit_by_name(instObj, "hw_keys_down", dkey);
		dkey = 0;
	}
	return TRUE;
#else
	return FALSE;
#endif
}

gboolean hw_keys_init(gchar *devName, gpointer instance) {
	gboolean ret = FALSE;
	keyOld.pg0 = 1;
	keyOld.pg1 = 1;
	keyOld.pg2 = 1;
	keyOld.pg3 = 1;
#ifdef __arm__
	gpioChip = gpiod_chip_open(devName);
	if (gpioChip) {
		g_signal_new("hw_keys_up", G_TYPE_OBJECT, G_SIGNAL_ACTION, 0, NULL, NULL, g_cclosure_marshal_VOID__UINT, G_TYPE_NONE, 1, G_TYPE_UINT);
		g_signal_new("hw_keys_down", G_TYPE_OBJECT, G_SIGNAL_ACTION, 0, NULL, NULL, g_cclosure_marshal_VOID__UINT, G_TYPE_NONE, 1, G_TYPE_UINT);
		pinPG0 = gpiod_chip_get_line(gpioChip, 192);
		if (pinPG0) {
			gpiod_line_request_input_flags(pinPG0, NULL, GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_UP);
			// gpiod_line_set_config(pinPG0, GPIOD_LINE_REQUEST_DIRECTION_INPUT, GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_UP, 0);
		} else { 
			debug("Get pin PG0 failed !");
		}

		pinPG1 = gpiod_chip_get_line(gpioChip, 193);
		if (pinPG1) {
			gpiod_line_request_input_flags(pinPG1, NULL, GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_UP);
			// gpiod_line_set_config(pinPG1, GPIOD_LINE_REQUEST_DIRECTION_INPUT, GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_UP, 0);
		} else { 
			debug("Get pin PG1 failed !");
		}

		pinPG2 = gpiod_chip_get_line(gpioChip, 194);
		if (pinPG2) {
			gpiod_line_request_input_flags(pinPG2, NULL, GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_UP);
			// gpiod_line_set_config(pinPG2, GPIOD_LINE_REQUEST_DIRECTION_INPUT, GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_UP, 0);
		} else { 
			debug("Get pin PG2 failed !");
		}

		pinPG3 = gpiod_chip_get_line(gpioChip, 195);
		if (pinPG3) {
			gpiod_line_request_input_flags(pinPG3, NULL, GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_UP);
			// gpiod_line_set_config(pinPG3, GPIOD_LINE_REQUEST_DIRECTION_INPUT, GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_UP, 0);
		} else { 
			debug("Get pin PG3 failed !");
		}

		g_timeout_add(50, get_key, instance);
	} else {
		debug("gpiochip0 open failed !");
	}
#endif
	return ret;
}

