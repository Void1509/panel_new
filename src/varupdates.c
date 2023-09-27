#include "app.h"
#include "debug.h"
#include "appVars.h"
#include "g_utils.h"

static void cmd60(uint8_t *data);
static void cmd61(uint8_t *data);
static void cmd62(uint8_t *data);
static void cmd63(uint8_t *data);
static void cmd64(uint8_t *data);
static void cmd65(uint8_t *data);
static void cmd66(uint8_t *data);
static void cmd67(uint8_t *data);
static void cmd68(uint8_t *data);
static void cmd69(uint8_t *data);
static void cmd6a(uint8_t *data);

static const struct {
	uint8_t cmd;
	void (*update)();
}upSel[] = {{0x60, cmd60}, {0x61, cmd61}, {0x62, cmd62}, {0x63, cmd63},
			{0x64, cmd64}, {0x65, cmd65}, {0x66, cmd66}, {0x67, cmd67},
			{0x68, cmd68}, {0x69, cmd69}, {0x6a, cmd6a}, {0, NULL}};

static void cmd60(uint8_t *data) {
}

static void cmd61(uint8_t *data) {
	bunkers_update(*data);
}

static void cmd62(uint8_t *data) {
}

static void cmd63(uint8_t *data) {
	plug_update(*data);
}

static void cmd64(uint8_t *data) {
	shaft_update(*data);
}

static void cmd65(uint8_t *data) {
	fans_update(*data);
}

static void cmd66(uint8_t *data) {
}

static void cmd67(uint8_t *data) {
}

static void cmd68(uint8_t *data) {
}

static void cmd69(uint8_t *data) {
}

static void cmd6a(uint8_t *data) {
}


static void parse_update(uint8_t *data, uint8_t len) {
	for(int i = 0; upSel[i].cmd > 0; i++) {
		if (*data == upSel[i].cmd) {
			upSel[i].update(data + 1);
			break;
		}
	}

}

void parse_MCU(uint8_t *data, uint8_t len) {
	
}

gboolean mcu_read_pipe(GIOChannel *ch, GIOCondition con, gpointer p) {
	union {
		struct PipesCMD_s cmd;
		char buf[256];
	}inPack;

	gsize rb;
	GError *err = NULL;
	g_io_channel_read_chars(ch, inPack.buf, (gsize)sizeof(struct PipesCMD_s), &rb, &err);
	if (inPack.cmd.len > 0) {
		g_io_channel_read_chars(ch, &inPack.buf[rb], (gsize)inPack.cmd.len, &rb, &err);
	}
//	g_io_channel_read_chars(ch, inPack.buf, 256, &rb, &err);
	if (err) {
		debugf("Pipe error:%s\n", err->message);
		g_error_free(err);
	} else {
		switch(inPack.cmd.cmd) {
			case pcmdUpdate:
					parse_update(inPack.cmd.args, inPack.cmd.len);
				break;
			case pcmdMCU:
					parse_MCU(inPack.cmd.args, inPack.cmd.len);
				break;
		}
	}

	return TRUE;
}

