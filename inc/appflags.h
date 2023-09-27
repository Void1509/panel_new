#ifndef __APPFLAGS_H__
#define __APPFLAGS_H__

struct AppFlags_s {
	unsigned mcuOnline:1;
	unsigned serialStart:1;
	unsigned serialSync:1;
};

#ifdef __MAIN_C__
struct AppFlags_s appFlags = {0};
#else
extern struct AppFlags_s appFlags;
#endif
#endif
