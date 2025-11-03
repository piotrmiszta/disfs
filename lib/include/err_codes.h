/*
 * Copyright (c) 2025 Piotr Miszta
 * SPDX-License-Identifier: MIT
 */

#ifndef DISFS_ERR_CODES_H_
#define DISFS_ERR_CODES_H_

#include <assert.h>
#include <errno.h>  // for errno
#include <string.h> // for strerror

typedef long long int err_t;

#define DISFS_SUCCESS 0
#define DISFS_ERR_GENERIC (-1)
#define DISFS_ERR_ALLOC (-2)
#define DISFS_ERR_SOCK (-3)
#define DISFS_ERR_EPOLL (-4)
#define DISFS_ERR_INVALID_ARG (-5)

#define DISFS_ERR_MAX_PEER (-10)
#define DISFS_ERR_READED (-11)
#define DISFS_ERR_FILE_GENERIC (-12)

#define ASSERT(cond, msg) assert(cond || (_Bool)msg)

#endif
