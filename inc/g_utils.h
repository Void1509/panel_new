#ifndef __MYG_UTILS_H__
#define __MYG_UTILS_H__
#include <glib.h>
#include <stdint.h>

#define PCMD_MAX	256

typedef enum {
	pcmdExit = 1,
	pcmdSend,
	pcmdUpdate,
	pcmdMCU,
	pcmdEnd = -1
}PipeCMD_e;

struct Signals_s {
	void *inst;
	const char *sigName;
	void *func;
	void *data;
};

struct GIOPipes_s {
	GIOChannel *in, *out;
};

struct PipesCMD_s {
	PipeCMD_e cmd:8;
	uint16_t len;
	uint8_t args[];
}__attribute__((packed));

void signals_connect(struct Signals_s *table);
void pipes_channel(struct GIOPipes_s *ps, int in, int out);
void pipes_send(int pd, const void *data, size_t len);
void mcu_pipe_send(struct PipesCMD_s *data);
#ifdef __MAIN_C__
struct GIOPipes_s mcuPipes;
#else
extern struct GIOPipes_s mcuPipes;
#endif
#endif
