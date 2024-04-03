#ifndef __ARGS_H
#define __ARGS_H

#include "common.h"

#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>

#define USAGE "\nUsage: %s <mode> -n <port number>\n\n"\
              "  -h : print this message\n\n"\
              "  mode:\n"\
              "    -p pool_size : use a thread pool of specified size to handle new requests\n"\
              "    -t : handle new connection in a new thread\n"\
              "    -c : handle new connection in a child process\n"


/* MODE takes 8-bits */
#define MASK_MODE   (0x07)
#define MODE_TPOOL  (0x01)
#define MODE_THREAD (0x02)
#define MODE_CHILDP (0x04)

typedef struct {
	uint8_t mode;
	uint8_t pool_size;
	uint16_t port;
} args_t;

/**
 * @brief Check the arguments
 *
 * @param argc Number of arguments
 * @param argv Pointer to arguments
 * @param args Pointer to args_t to store the parsed arguments
 *
 * @return OK on success, ERR on failure.
 */
int check_args(int argc, char * const * argv, args_t * args);

#endif
