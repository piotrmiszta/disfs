#ifndef DISFS_FILE_MENAGER_H_
#define DISFS_FILE_MENAGER_H_

#include <stdint.h>
#include <time.h>

#define FILE_MENAGER_READ_FLAG (1U << 0)
#define FILE_MENAGER_WRITE_FLAG (1U << 1)
#define FILE_MENAGER_EXEC_FLAG (1U << 2)

#define MAX_FILE_NAME_LEN 20
#define SHA1_LENGTH 20

typedef struct file_t
{
    char name[MAX_FILE_NAME_LEN];
    uint8_t sha1[SHA1_LENGTH];
    time_t created;
    time_t modified;
    uint32_t permission;
} file_t;

int path_exist(const char* path);
int path_permission(const char* path);

#endif
