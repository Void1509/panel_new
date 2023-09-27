#ifndef __DEBUG_H__
#define __DEBUG_H__
#include <stdio.h>

#define debugf(fmt, args...)		fprintf(stdout, "File:%s line:%u " fmt, __BASE_FILE__, __LINE__, args)
#define debug(fmt)		fprintf(stdout, "File:%s line:%u " fmt, __BASE_FILE__, __LINE__)

#endif
