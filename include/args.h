#ifndef __ARGS_H
#define __ARGS_H

#include "common.h"

#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>

#define USAGE "\nUsage: %s <handler> -n <port number>\n\n"\
              "  -h : print this message\n\n"\
              "  handler:\n"\
              "    -p pool_size : use a thread pool of specified size to handle new requests\n"\
              "    -t : handle new connection in a new thread\n"\
              "    -c : handle new connection in a child process\n"

/**
 * MODE_UNDEF : default value
 * MODE_TPOOL : use a thread pool of specified size to handle new requests.
 * MODE_THREAD : handle new connection in a new thread.
 * MODE_CHILDP : handle new connection in a child process.
 */
typedef enum {
	MODE_UNDEF,
	MODE_TPOOL,
	MODE_THREAD,
	MODE_CHILDP
} handler_t;

typedef struct {
	handler_t handler;
	uint32_t pool_size;
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
