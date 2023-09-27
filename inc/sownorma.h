#ifndef __SOWNORMA_H__
#define __SOWNORMA_H__
#include "app.h"

struct SowNorma_s {
	GtkBox *main;
};

#ifdef __SOWNORMA_OPT_C__
struct SowNorma_s sowNorma;
#else
extern struct SowNorma_s sowNorma;
#endif

void sownorma_option_init();

#endif
