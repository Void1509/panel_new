#ifndef __APPTYPES_H__
#define __APPTYPES_H__
#include <stdint.h>

typedef struct {
	int in, out;
}PipeSock_t;

struct PipePair_s {
	PipeSock_t pipe[2];
};

#endif
