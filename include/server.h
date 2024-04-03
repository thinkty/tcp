#ifndef __SERVER_H
#define __SERVER_H

#include "common.h"
#include "tcp.h"
#include "args.h"

#include <signal.h>
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>

/**
 * @brief Start accepting connections using the threads in the pool.
 *
 * @param sock TCP socket file descriptor
 * @param pool_size Number of threads to use in the pool
 */
void start_tpool(int sock, uint32_t pool_size);

/**
 * @brief Start accepting connections and spawn new threads.
 *
 * @param sock TCP socket file descriptor
 */
void start_thread(int sock);

/**
 * @brief Start accepting connections and spawn child processes.
 *
 * @param sock TCP socket file descriptor
 */
void start_childp(int sock);

/**
 * @brief Task to accept and handle new connections.
 *
 * @param sock TCP socket file descriptor
 *
 * @return NULL
 */
void * accept_task(void * sock);

/**
 * @brief Wrapper of echo to pass to a thread.
 *
 * @param sock TCP socket file descriptor
 *
 * @return NULL
 */
void * echo_task(void * csock);

/**
 * @brief Read from client socket and echo.
 *
 * @param csock TCP client socket file descriptor
 */
void echo(int csock);

#endif

