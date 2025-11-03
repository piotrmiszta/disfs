/*
 * Copyright (c) 2025 Piotr Miszta
 * SPDX-License-Identifier: MIT
 */

#include "file_menager.h"
#include "err_codes.h"
#include "logger.h"
#include <linux/limits.h>
#include <sys/inotify.h>
#include <unistd.h>

int path_exist(const char* path)
{
    if (access(path, F_OK) == 0)
    {
        return 1;
    }
    return 0;
}

int path_permission(const char* path)
{
    int permission = 0;
    if (access(path, R_OK) == 0)
    {
        permission |= FILE_MENAGER_READ_FLAG;
    }
    if (access(path, W_OK) == 0)
    {
        permission |= FILE_MENAGER_WRITE_FLAG;
    }
    if (access(path, X_OK) == 0)
    {
        permission |= FILE_MENAGER_EXEC_FLAG;
    }
    return permission;
}

/* TODO: this function should be active in parrarell
   but need to make some comunication between this function
   and threads that are sending files.
*/

void* file_watcher_thread(void* arg)
{
    struct file_watcher_params_s* param = arg;
    assert(param != NULL);

    const char* path = param->path;

    int32_t fd = inotify_init1(IN_NONBLOCK);
    if (fd < 0)
    {
        LOG_ERROR("Cannot open inotify event\n");
        return NULL;
    }
    int32_t wd = inotify_add_watch(fd, path, IN_CREATE | IN_DELETE | IN_MODIFY);
    if (wd < 0)
    {
        LOG_ERROR("Cannot add watch for directory %s, err = %d, %s\n", path,
                  errno, strerror(errno));
        return NULL;
    }

    param->run = 1;
    while (param->run)
    {
        size_t event_buff_len = NAME_MAX + 1 + sizeof(struct inotify_event);
        char buffer[event_buff_len];
        int length = read(fd, buffer, event_buff_len);
        int i = 0;
        while (i < length)
        {
            struct inotify_event* ev = (struct inotify_event*)&buffer[i];
            if (ev->len)
            {
                if (ev->mask & IN_CREATE)
                {
                    LOG_DEBUG("Created %s file\n", ev->name);
                }
                if (ev->mask & IN_DELETE)
                {
                    LOG_DEBUG("Deleted %s file\n", ev->name);
                }

                if (ev->mask & IN_MODIFY)
                {
                    LOG_DEBUG("Modified %s file\n", ev->name);
                }
            }
            i += sizeof(struct inotify_event) + ev->len;
        }
    }
    return NULL;
}

void file_watcher_end_thread(file_watcher_params_t args[static 1])
{
    args->run = 0;
    pthread_join(args->thread, NULL);
}
