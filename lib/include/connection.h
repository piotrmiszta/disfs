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
} client_t;

typedef struct connection_t
{
    struct sockaddr_in addr;
    socklen_t addr_len;
    int32_t fd;
    client_t clients[MAX_NEIGHBOURS];

    int32_t udp_fd;
    struct sockaddr_in udp_addr;
} connection_t;

err_t create_connection(connection_t connection[static 1], int32_t port);

#endif
