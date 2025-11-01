#include "file_menager.h"
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
