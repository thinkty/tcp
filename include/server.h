#ifndef __SERVER_H
#define __SERVER_H

#include "common.h"
#include "tcp.h"

#include <stdio.h>
#include <stdint.h>
#include <pthread.h>

/**
 * @brief Check the arguments.
 *
 * @param argc Number of arguments
 * @param argv Pointer to arguments
 * @param port Pointer to port number
 * @param pool_size Pointer to thread pool size
 *
 * @return OK on success, ERR on failure.
 */
int check_args(int argc, char * argv[], uint16_t * port, uint32_t * pool_size);

/**
 * @brief Accept and handle new connections.
 *
 * @param sock TCP socket
 *
 * @return NULL
 */
void * server_accept(void * sock);

/**
 * @brief Read from client socket and echo.
 *
 * @param csock TCP client socket
 */
void handle(int csock);

#endif

