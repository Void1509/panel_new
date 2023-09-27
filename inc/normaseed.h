#ifndef __NORMASEED_H__
#define __NORMASEED_H__
#include "app.h"

struct NormaSeed_s {
	GtkBox *main;
	GtkEntry *entKulturaName, *entSemyanGa;
	GtkButton *appExit;
};

#ifdef __NORMASEED_C__
struct NormaSeed_s normaSeed;
#else
extern struct NormaSeed_s normaSeed;
#endif

void norma_seed_init();

#endif
