#include "lib.h"
#include "logger.h"
#include <assert.h>

int test(int x)
{
    LOG_INFO("Running test function!\n");
    LOG_WARNING("Running test function!\n");
    LOG_ERROR("Running test function!\n");
    LOG_DEBUG("Running test function!\n");
    LOG_TRACE("Running test function!\n");
    return 5;
}
