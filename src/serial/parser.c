#include <stdint.h>
#include <stdio.h>
#include <alloca.h>
#include <string.h>
#include "g_utils.h"
#include "mcu_data.h"
#include "debug.h"

typedef uint8_t *(*ParseFunc)(uint8_t *b, uint8_t *e);

static uint8_t *ack_fn60(uint8_t *bf, uint8_t *end);
static uint8_t *ack_fn61(uint8_t *bf, uint8_t *end);
static uint8_t *ack_fn62(uint8_t *bf, uint8_t *end);
static uint8_t *ack_fn63(uint8_t *bf, uint8_t *end);
static uint8_t *ack_fn64(uint8_t *bf, uint8_t *end);
static uint8_t *ack_fn65(uint8_t *bf, uint8_t *end);
static uint8_t *ack_fn66(uint8_t *bf, uint8_t *end);
static uint8_t *ack_fn67(uint8_t *bf, uint8_t *end);
static uint8_t *ack_fn68(uint8_t *bf, uint8_t *end);
static uint8_t *ack_fn69(uint8_t *bf, uint8_t *end);
static uint8_t *ack_fn6A(uint8_t *bf, uint8_t *end);
static uint8_t *ack_none(uint8_t *bf, uint8_t *end);

struct AckFunc {
	uint8_t type;
	ParseFunc pars;
//	uint8_t *(*pars)(uint8_t *buf, uint8_t *end);
}ackTable[] = {
	{0x60, ack_fn60},
	{0x61, ack_fn61},
	{0x62, ack_fn62},
	{0x63, ack_fn63},
	{0x64, ack_fn64},
	{0x65, ack_fn65},
	{0x66, ack_fn66},
	{0x67, ack_fn67},
	{0x68, ack_fn68},
	{0x69, ack_fn69},
	{0x6A, ack_fn6A},
	{0, ack_none}
};

static uint8_t *ack_fn60(uint8_t *bf, uint8_t *end) {
	if ((bf + 4) <= end) {
		int inx = bf[1] & 3;
		mcuData.scale[inx] = (((uint16_t)bf[2]) << 8) | bf[3];
//		bf += 4;
	}
	return bf;
}

static uint8_t *ack_fn61(uint8_t *bf, uint8_t *end) {
	if ((bf + 4) <= end) {
		uint8_t inx = bf[1] & 3;
		mcuData.bunkers[inx] = (bf[2] << 4) | bf[3];
		struct PipesCMD_s *cmd = alloca(sizeof(struct PipesCMD_s) + 2);
		cmd->cmd = pcmdUpdate;
		cmd->len = 2;
		cmd->args[0] = 0x61;
		cmd->args[1] = inx;
		mcu_pipe_send(cmd);
	}
	return bf;
}

static uint8_t *ack_fn62(uint8_t *bf, uint8_t *end) {
	if ((bf + 4) <= end) {
		uint8_t inx = bf[1] & 3;
		mcuData.psens[inx] = bf[2];
		struct PipesCMD_s *cmd = alloca(sizeof(struct PipesCMD_s) + 2);
		cmd->cmd = pcmdUpdate;
		cmd->len = 2;
		cmd->args[0] = 0x62;
		cmd->args[1] = inx;
		mcu_pipe_send(cmd);
	}
	return bf;
}

static uint8_t *ack_fn63(uint8_t *bf, uint8_t *end) {
	if ((bf + 4) <= end) {
		mcuData.dsens = bf[2];
		struct PipesCMD_s *cmd = alloca(sizeof(struct PipesCMD_s) + 1);
		cmd->cmd = pcmdUpdate;
		cmd->len = 1;
		cmd->args[0] = 0x63;
		mcu_pipe_send(cmd);
	}
	return bf;
}

static uint8_t *ack_fn64(uint8_t *bf, uint8_t *end) {
	if ((bf + 4) <= end) {
		uint8_t inx = bf[1] & 3;
		mcuData.shaft[inx] = (((uint16_t)bf[2]) << 8) | bf[3];
//		debugf("Shaft:%4X dump: %2hhx %2hhx %2hhx %2hhx\n", mcuData.shaft[inx], bf[0], bf[1], bf[2], bf[3]);
		struct PipesCMD_s *cmd = alloca(sizeof(struct PipesCMD_s) + 2);
		cmd->cmd = pcmdUpdate;
		cmd->len = 2;
		cmd->args[0] = 0x64;
		cmd->args[1] = inx;
		mcu_pipe_send(cmd);
	}
	return bf;
}

static uint8_t *ack_fn65(uint8_t *bf, uint8_t *end) {
	if ((bf + 4) <= end) {
		uint8_t inx = bf[1] & 3;
		mcuData.fans[inx] = (((uint16_t)bf[2]) << 8) | bf[3];
		struct PipesCMD_s *cmd = alloca(sizeof(struct PipesCMD_s) + 2);
		cmd->cmd = pcmdUpdate;
		cmd->len = 2;
		cmd->args[0] = 0x65;
		cmd->args[1] = inx;
		mcu_pipe_send(cmd);
	}
	return bf;
}

static uint8_t *ack_fn66(uint8_t *bf, uint8_t *end) {
	if ((bf + 4) <= end) {
		switch(bf[1]) {
			case 0:
				mcuData.enSpeed.count = (((uint16_t)bf[2]) << 8) | bf[3];
				break;
			case 1:
				mcuData.enSpeed.ms = (((uint16_t)bf[2]) << 8) | bf[3];
				break;
		}
		struct PipesCMD_s *cmd = alloca(sizeof(struct PipesCMD_s) + 2);
		cmd->cmd = pcmdUpdate;
		cmd->len = 2;
		cmd->args[0] = 0x66;
		cmd->args[1] = bf[1];
		mcu_pipe_send(cmd);
	}
	return bf;
}

static uint8_t *ack_fn67(uint8_t *bf, uint8_t *end) {
	if ((bf + 4) <= end) {
		int b = (bf[1] >> 4); // номер башни
		int s = (bf[1] & 0xf); // номер семяпровода
		mcuData.seminal[b][s] = (((uint16_t)bf[2]) << 8) | bf[3];
		struct PipesCMD_s *cmd = alloca(sizeof(struct PipesCMD_s) + 2);
		cmd->cmd = pcmdUpdate;
		cmd->len = 2;
		cmd->args[0] = 0x67;
		cmd->args[1] = bf[1];
		mcu_pipe_send(cmd);
	}
	return bf;
}

static uint8_t *ack_fn68(uint8_t *bf, uint8_t *end) {
	if ((bf + 4) <= end) {

		mcuData.volt = (((uint16_t)bf[2]) << 8) | bf[3];
		struct PipesCMD_s *cmd = alloca(sizeof(struct PipesCMD_s) + 1);
		cmd->cmd = pcmdUpdate;
		cmd->len = 1;
		cmd->args[0] = 0x68;
		mcu_pipe_send(cmd);
	}
	return bf;
}

static uint8_t *ack_fn69(uint8_t *bf, uint8_t *end) {
	if ((bf + 4) <= end) {
		mcuData.fuel = (((uint16_t)bf[2]) << 8) | bf[3];
		struct PipesCMD_s *cmd = alloca(sizeof(struct PipesCMD_s) + 1);
		cmd->cmd = pcmdUpdate;
		cmd->len = 1;
		cmd->args[0] = 0x69;
		mcu_pipe_send(cmd);
	}
	return bf;
}

static uint8_t *ack_fn6A(uint8_t *bf, uint8_t *end) {
	if ((bf + 4) <= end) {
		if (bf[1] < 5) {
			mcuData.service[bf[1]] = (((uint16_t)bf[2]) << 8) | bf[3];
			struct PipesCMD_s *cmd = alloca(sizeof(struct PipesCMD_s) + 2);
			cmd->cmd = pcmdUpdate;
			cmd->len = 2;
			cmd->args[0] = 0x6a;
			cmd->args[1] = bf[1];
			mcu_pipe_send(cmd);
		}
	}
	return bf;
}

static uint8_t *ack_none(uint8_t *bf, uint8_t *end) {
	return(bf + 4);
}

static ParseFunc pars_sel(uint8_t ack) {
	int inx = 0;
	while(ackTable[inx].type) {
		if (ack == ackTable[inx].type)
			break;
		inx++;
	}
	return ackTable[inx].pars;
}

void hex_dump(unsigned char *buf, unsigned short len, unsigned char col) {
	unsigned inx = 0;
	uint8_t cl = 0;

	fprintf(stdout, "%04X : ", inx);
	while(inx < len) {
		fprintf(stdout, "%02x ", buf[inx++]);
		cl++;
		if (cl == col) {
			cl = 0;
			fprintf(stdout, "\n%04X : ", inx);
		}
	}
	fprintf(stdout, "\r\n");
}

void parse_buf_func(struct ReadBuf_s *rb) {
//	hex_dump(rb->buf, rb->inx, 4);

	uint8_t *st = rb->buf;
	uint8_t *end = &rb->buf[rb->inx];

	while(st < end) {
		st = (pars_sel(*st)(st, end)) + 4;
	}

//	appConfig.ans = 0;
//	appConfig.update = 0;
}

