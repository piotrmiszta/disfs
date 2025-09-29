#include "connection.h"
#include "logger.h"
#include <assert.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    LOG_DEBUG("Hello World from %s!\n", argv[0]);
    connection_t conn;
    create_connection(&conn, 8080);
    return 0;
}
