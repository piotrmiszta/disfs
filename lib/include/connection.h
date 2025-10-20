/*
 * Copyright (c) 2025 Piotr Miszta
 * SPDX-License-Identifier: MIT
 */

#ifndef DISFS_CONNECTION_H_
#define DISFS_CONNECTION_H_

#include "err_codes.h"
#include <netinet/in.h>
#include <stdint.h>

#define MAX_NEIGHBOURS 5

typedef struct client_t
{
    struct sockaddr_in addr;
    socklen_t len;
    int32_t fd;
    int_fast8_t active;
    char ip[INET_ADDRSTRLEN];
} client_t;

typedef struct connection_t
{
    struct sockaddr_in addr;
    socklen_t addr_len;
    int32_t fd;
    client_t clients[MAX_NEIGHBOURS];

    char local_ip[INET_ADDRSTRLEN];

    int32_t udp_fd;
    struct sockaddr_in udp_addr;

    pthread_t udp_th;
    pthread_t tcp_th;

    volatile int udp_th_run;
    volatile int tcp_th_run;

} connection_t;

/**
 * @brief optional params for connection create
 */
typedef struct
{
    int32_t port_tcp;
    int32_t port_udp;
} connection_params_opt;

err_t _internal_create_connection(connection_t conn[static 1],
                                  connection_params_opt params);

void close_connection(connection_t conn[static 1]);

#define create_connection(conn, ...)                                           \
    _internal_create_connection(conn, (connection_params_opt){__VA_ARGS__})

#endif
