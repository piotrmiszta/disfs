/*
 * Copyright (c) 2025 Piotr Miszta
 * SPDX-License-Identifier: MIT
 */

#include "connection.h"
#include "file_menager.h"
#include "logger.h"
#include <assert.h>
#include <stdio.h>
#include <unistd.h>

#define VALGRIND

const char default_path[] = "/home/ubuntu/disfs";

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    const char* path;
    if (argc == 1)
    {
        path = default_path;
    }
    else
    {
        path = argv[1];
    }

    file_watcher_params_t params = {.path = path, .run = 1};
    LOG_DEBUG("Path for DISFS: %s\n", path);

    int permission = path_permission(path);
    LOG_DEBUG("Path %s exist = %d, read = %d, write = %d, exec = %d\n", path,
              path_exist(path), permission & FILE_MENAGER_READ_FLAG,
              permission & FILE_MENAGER_WRITE_FLAG,
              permission & FILE_MENAGER_EXEC_FLAG);

    //    LOG_DEBUG("Hello World from %s!\n", argv[0]);
    pthread_create(&params.thread, NULL, file_watcher_thread, &params);

    connection_t conn = {0};
    create_connection(&conn, .port_udp = 8000);

#ifdef VALGRIND
    sleep(10);
    close_connection(&conn);
    file_watcher_end_thread(&params);
#else
    while (1)
    {
    }
#endif
    return 0;
}
