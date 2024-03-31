#ifndef __COMMON_H
#define __COMMON_H

#define eprintf(format, ...) \
	do { fprintf(stderr, format "\n" __VA_OPT__(,) __VA_ARGS__); } while (0)

#define OK  (0)
#define ERR (-1)

#endif
