#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>
#include <glib.h>
#include <unistd.h>
#include "app.h"
#include "appVars.h"
#include "appflags.h"
#include "settings.h"
#include "mcu_connect.h"
#include "mcu_data.h"
#include "g_utils.h"
#include "debug.h"

void parse_buf_func(struct ReadBuf_s *rb);

static GMainContext *ctx = NULL;
static GIOChannel *sPort = NULL;
static GIOChannel *pIn = NULL;
static GIOChannel *pOut = NULL;
static GMainLoop *mLoop = NULL;

static struct MCUConnect_s mcu;

static void pcmd_exit(struct PipesCMD_s *cmd);
static void pcmd_send(struct PipesCMD_s *cmd);

struct {
	int16_t cmd;
	void (*func)(struct PipesCMD_s *pcmd);
}pipeCMDs[] = {{pcmdExit, pcmd_exit}, {pcmdSend, pcmd_send}, {pcmdEnd, NULL}};

static int baud_set(uint32_t baud) {
	int ret = -1;

	switch(baud) {
		case 9600:
			ret = B9600;
		break;
		case 19200:
			ret = B19200;
		break;
		case 38400:
			ret = B38400;
		break;
		case 57600:
			ret = B57600;
		break;
		case 115200:
			ret = B115200;
		break;
		case 230400:
			ret = B230400;
		break;
		case 460800:
			ret = B460800;
		break;
		case 921600:
			ret = B921600;
		break;
	}

	return ret;
}

static void set_tty(int fd, int ts) {
	struct termios tc;

	tcgetattr(fd, &tc);
	tc.c_iflag &= ~(BRKINT | ICRNL | IMAXBEL | INLCR | IGNBRK | IGNCR | IXON);
	tc.c_oflag &= ~(OPOST | ONLCR);
	tc.c_cflag &= ~(CSIZE | PARENB);
	tc.c_cflag |= CS8;
	tc.c_lflag &= ~(ECHO | ICANON | ISIG | ECHOE);
	tc.c_cc[VMIN] = 1;
	tc.c_cc[VTIME] = 1;
	cfsetispeed(&tc, ts);
	cfsetospeed(&tc, ts);
	tcsetattr(fd, TCSANOW, &tc);
}

static void buf_sync(struct ReadBuf_s *rb) {

	if (rb->inx > 3) {
		if (!memcmp(&rb->buf[rb->inx - 4], "\x40\x40\x40\x40", 4)) {
			rb->inx = 0;
			appFlags.serialSync = 1;
		}
	}
}

static gboolean read_port_cb(gpointer p) {
	GIOChannel *in = p;
	GError *err = NULL;
	gsize rb;

	g_io_channel_read_chars(in, &readBuf.buf[readBuf.inx], 1, &rb, &err);
	
	if (err) {
		debugf("Read MCU port error:%s\n", err->message);
		g_error_free(err);
	} else {
		readBuf.inx += rb;
		if (readBuf.inx > 3) {
			if (appFlags.serialSync) {
				parse_buf_func(&readBuf);
				readBuf.inx = 0;
			} else {
				buf_sync(&readBuf);
			}
		}
		if (readBuf.inx == RBUF_MAX)
			readBuf.inx = 0;
	}
	return TRUE;
}

static void pcmd_exit(struct PipesCMD_s *cmd) {
	debugf("MCU Thread quit! sz:%d\n", (int)sizeof(struct PipesCMD_s));
	g_main_loop_quit(mLoop);
}

static void pcmd_send(struct PipesCMD_s *cmd) {
	gsize wb;
	GError *err = NULL;
	g_io_channel_write_chars(sPort, cmd->args, cmd->len, &wb, &err);
	if (err) {
		debugf("Error write serial port:%s %d len:%hx\n", err->message, err->code, cmd->len);
		g_error_free(err);
	} else {
		g_io_channel_flush(sPort, &err);
		if (err)
			g_error_free(err);
	}
}

static void cmd_parse(struct PipesCMD_s *cmd) {
	for(int i = 0; pipeCMDs[i].cmd != pcmdEnd; i++) {
		if (cmd->cmd == pipeCMDs[i].cmd) {
			pipeCMDs[i].func(cmd);
			break;
		}
	}
}

static gboolean read_pIn_cb(gpointer p) {
	GIOChannel *in = p;
	union {
		struct PipesCMD_s cmd;
		char buf[256];
	}inPack;

	gsize rb;
	GError *err = NULL;
	g_io_channel_read_chars(in, inPack.buf, 256, &rb, &err);
	if (err) {
		debugf("Pipe error:%s\n", err->message);
		g_error_free(err);
	} else {
		cmd_parse(&inPack.cmd);
	}
	return TRUE;
}

static void serial_loop(struct MCUConnect_s *mcu) {
	ctx = g_main_context_new();
	sPort = g_io_channel_unix_new(mcu->fd);
	g_io_channel_set_encoding(sPort, NULL, NULL);
	{
		struct GIOPipes_s pp;
		pipes_channel(&pp, mcu->pIn, mcu->pOut);
		pIn = pp.in;
		pOut = pp.out;
	}
/*
	pIn = g_io_channel_unix_new(mcu->pIn);
	pOut = g_io_channel_unix_new(mcu->pOut);
	g_io_channel_set_encoding(pIn, NULL, NULL);
	g_io_channel_set_encoding(pOut, NULL, NULL);
*/
	g_io_channel_set_close_on_unref(sPort, TRUE);
	GSource *sPortSrc, *pInSrc;
	sPortSrc = g_io_create_watch(sPort, G_IO_IN | G_IO_PRI | G_IO_HUP);
	pInSrc = g_io_create_watch(pIn, G_IO_IN);
	g_source_set_callback(sPortSrc, read_port_cb, sPort, NULL);
	g_source_set_callback(pInSrc, read_pIn_cb, pIn, NULL);
	g_source_attach(sPortSrc, ctx);
	g_source_attach(pInSrc, ctx);
	mLoop = g_main_loop_new(ctx, FALSE);

	debug("MCU serial thread start\n");
	g_main_loop_run(mLoop);

	g_main_loop_unref(mLoop);
	g_main_context_unref(ctx);
	g_io_channel_unref(sPort);
	ctx = NULL;
	sPort = NULL;
	mLoop = NULL;
}

gpointer mcu_serial_thread(gpointer data) {
	struct MCUConnect_s *cn = data;

	serial_loop(cn);
	free(cn);
	return NULL;
}

gboolean mcu_serial_init(struct MCUConnect_s *cn) {
	gboolean ret = TRUE;
	cn->status = MCU_Ok;
	int baud;

	if (!appSettings.PortControl.dev) {
		cn->fd = -1;
		cn->status = ErrorNoPort;
		ret = FALSE;
	}
	if (ret) {

//		cn->fd = open(appSettings.PortControl.dev, O_RDWR| O_NOCTTY);
		cn->fd = open(appSettings.PortControl.dev, O_RDWR| O_NOCTTY | O_NONBLOCK| O_SYNC);

		if (cn->fd < 1) {
			cn->status = ErrorOpenPort;
			ret = FALSE;
		} else {
			baud = baud_set(appSettings.PortControl.speed);
			if (baud < 0) {
				cn->status = ErrorBaud;
				close(cn->fd);
				ret = FALSE;
			}
			if (ret) {
				set_tty(cn->fd, baud);
				appFlags.mcuOnline = TRUE;
				appFlags.serialStart = TRUE;
			}
		}
	}
	return ret;
}

// ################################################################################################
/*
void mcu_send_cmd(uint8_t cmd, uint8_t *data, uint8_t len) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
	size_t sz = sizeof(MCUCmdPack_t) + len;
	MCUCmdPack_t *pk = alloca(sz);
	pk->mark = MCUCmdMark;
	pk->cmd = cmd;
	pk->len = len;
	memcpy(pk->data, data, (size_t)len);
	write(appVars.mcuPipe.pipe[0].out, (const void*)pk, sz);
#pragma GCC diagnostic pop
}
*/
void mcu_pipe_send(struct PipesCMD_s *data) {
	gsize wb;
	GError *err = NULL;
	g_io_channel_write_chars(pOut, (void*)data, (gssize)(sizeof(struct PipesCMD_s)+data->len), &wb, &err);
	if (err) {
		debugf("Send pipe error:%s\n", err->message);
		g_error_free(err);
	} else {
		g_io_channel_flush(pOut, &err);
		if (err)
			g_error_free(err);
	}
}
