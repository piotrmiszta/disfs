/*
 * Copyright (c) 2025 Piotr Miszta
 * SPDX-License-Identifier: MIT
 */

#include "connection.h"
#include "logger.h"
#include <assert.h>
#include <stdio.h>
#include <unistd.h>

#define VALGRIND

int main(int argc, char* argv[])
{
    /* send UDP packet */

    /* first we should initiate connection */
    LOG_DEBUG("Hello World from %s!\n", argv[0]);
    connection_t conn = {};
    create_connection(&conn, .port_udp = 8000);

#ifdef VALGRIND
    sleep(10);
    close_connection(&conn);
#else
    while (1)
    {
    }
#endif
    return 0;
}
