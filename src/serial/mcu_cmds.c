#include "g_utils.h"
#include "appflags.h"
#include "appVars.h"
#include "settings.h"
#include "mcu_data.h"
#include "debug.h"

static const struct MCUProtocolCmd_s constMCUProtCmd = {0x21, 0, 0};	//Эталон протокола
static const uint8_t cmd41[] = {4, 0x41,99, 99, 4, 0x11};
static const uint8_t cmd42[] = {1, 0x42,0};
static const uint8_t cmd43[] = {4, 0x43,0x50, 0x64, 7, 0x1};
static const uint8_t cmd44[] = {1, 0x44,0};

static const uint8_t _start[] = {0x40,0x40,0x40,0x40};

static struct MCUProtocolCmd_s *build_pack(const uint8_t *data) {
	uint8_t sz = data[0];
	struct MCUProtocolCmd_s *ret = malloc(sizeof(struct MCUProtocolCmd_s) + sz);
	memcpy(ret, &constMCUProtCmd, sizeof(constMCUProtCmd));
	ret->type = data[1];
	ret->len = sz;
	if (sz) {
		memcpy(ret->data, &data[2], sz);
		switch(ret->type) {
			case 0x43:
				ret->data[0] = appSettings.BrightnessScreen;
				ret->data[1] = appSettings.BrightnessButtons;
				ret->data[3] |= (((int)settings_get_speed()) && mcuData.dsens)?0x10:0;
				break;
		}
	}
	return ret;
}

static void mcu_send_cmd_start() {
	appFlags.serialStart = FALSE;
	struct {
		struct PipesCMD_s cmd;
		uint8_t data[4];
	}sendCMD = {.cmd = {pcmdSend, 4}};
	memcpy(sendCMD.cmd.args, _start, 4);

	pipes_send(appVars.mcuPipe.pipe[0].out, (void*)&sendCMD, sizeof(sendCMD));
//	debugf("MCU Command Start; Len:%d\n", (int)sizeof(sendCMD));
}

void mcu_send_commands() {
	if (appFlags.serialStart) {
		mcu_send_cmd_start();
		return;
	} else {
		struct MCUProtocolCmd_s *pack = build_pack(cmd43);
		int sz = sizeof(struct MCUProtocolCmd_s) + pack->len;
		struct PipesCMD_s *cmd = malloc(sizeof(struct PipesCMD_s) + sz);
		memcpy(cmd->args, pack, sz);
		cmd->cmd = pcmdSend;
		cmd->len = sz;
		pipes_send(appVars.mcuPipe.pipe[0].out, (void*)cmd, sizeof(struct PipesCMD_s) + sz);

		free(cmd);
//		write(fd, pack, sizeof(struct MCUProtocolCmd_s) + pack->len);
		free(pack);
	}
}
