#ifndef __APPVARS_H__
#define __APPVARS_H__
#include "apptypes.h"

struct AppVars_s {
	struct PipePair_s mcuPipe;
	void *mcuThread;
};

#ifdef __MAIN_C__
struct AppVars_s appVars;
#else
extern struct AppVars_s appVars;
#endif

#endif
