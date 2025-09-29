/*
 * Copyright (c) 2025 Piotr Miszta
 * SPDX-License-Identifier: MIT
 */

#include "connection.h"
#include "err_codes.h"
#include "logger.h"
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/epoll.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

#define EPOLL_MAX_FD 100

static void* connection_thread(void* arg);
static err_t connection_set_noblock(int32_t fd);
static err_t connection_add_event(int32_t epoll, int32_t fd, uint32_t state);
static err_t connection_accept_client(int32_t epoll,
                                      connection_t connection[static 1]);
static err_t connection_handle_events(int32_t epoll, struct epoll_event* events,
                                      int32_t events_count,
                                      connection_t connection[static 1]);

err_t create_connection(connection_t connection[static 1], int32_t port)
{
    /* create udp socket */
    connection->udp_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (connection->udp_fd <= 0)
    {
        LOG_ERROR("Cannot create socket for udp connection: errno=%d : %s\n",
                  errno, strerror(errno));
        return DISFS_ERR_SOCK;
    }

    int32_t udp_opt = -1;
    int32_t ret = setsockopt(connection->udp_fd, SOL_SOCKET, SO_REUSEADDR,
                             &udp_opt, sizeof(udp_opt));

    if (ret < 0)
    {
        LOG_ERROR(
            "Cannot set options to socket: socket = %d, errno = %d : %s!\n",
            connection->udp_fd, errno, strerror(errno));
        return DISFS_ERR_SOCK;
    }

    connection->udp_addr.sin_family = AF_INET;
    connection->udp_addr.sin_addr.s_addr = INADDR_ANY;
    connection->udp_addr.sin_port = htons(8080);

    if (bind(connection->udp_fd, (struct sockaddr*)&connection->udp_addr,
             sizeof(connection->udp_addr)) < 0)
    {
        LOG_ERROR("Cannot bind socket to port: port %d errno: %d : %s!\n", port,
                  errno, strerror(errno));
        return DISFS_ERR_SOCK;
    }

    connection->fd = socket(AF_INET, SOCK_STREAM, 0);
    if (connection->fd <= 0)
    {
        LOG_ERROR("Cannot create socket for connection errno: %d : %s!\n",
                  errno, strerror(errno));
        return DISFS_ERR_SOCK;
    }

    int32_t opt = -1;
    ret =
        setsockopt(connection->fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (ret < 0)
    {
        LOG_ERROR("Cannot set options to socket: errno: %d : %s!\n", errno,
                  strerror(errno));
        return DISFS_ERR_SOCK;
    }

    connection->addr.sin_addr.s_addr = INADDR_ANY;
    connection->addr.sin_family = AF_INET;
    connection->addr.sin_port = htons(port);
    connection->addr_len = sizeof(connection->addr);

    ret = bind(connection->fd, (struct sockaddr*)&connection->addr,
               connection->addr_len);
    if (ret < 0)
    {
        LOG_ERROR("Cannot bind socket to port: port %d errno: %d : %s!\n", port,
                  errno, strerror(errno));
        return DISFS_ERR_SOCK;
    }

    ret = listen(connection->fd, 3);
    if (ret < 0)
    {
        LOG_ERROR("Cannot listen for socket errno: %d : %s!\n", errno,
                  strerror(errno));
        return DISFS_ERR_SOCK;
    }

    LOG_DEBUG("Successfully created connection socket: %d\n", connection->fd);

    /*
       TODO: this server should be able to handle multiple connections
       to do this implement epoll from linux.
       This architecture will be peer to peer connection, after establishing
       this connection, be sure that we don't have another open conection.
       For simplicity at start let's implement each to each connection
       mechanism, but the best option is to handle connection to only 3/4
       neighbours, with lowest ping/ max throuput and reoute messages to all
       other peers. This connection should be established after multicast
       message received
    */

    pthread_t th;
    pthread_create(&th, NULL, connection_thread, connection);
    while (1)
    {
    }
    return 0;
}

static void* connection_thread(void* arg)
{
    ASSERT(arg, "Argument for thread function cannot be nullptr");
    int32_t epoll = epoll_create(EPOLL_MAX_FD);
    struct epoll_event events[EPOLL_MAX_FD * 10]; // TODO: remove magic number
    connection_t* conn = arg;
    connection_set_noblock(conn->fd);
    connection_add_event(epoll, conn->fd, EPOLLIN);
    connection_add_event(epoll, conn->udp_fd, EPOLLIN);
    while (1)
    {
        int32_t no_events = epoll_wait(epoll, events, EPOLL_MAX_FD * 10, 1000);
        connection_handle_events(epoll, events, no_events, conn);
    }
    return NULL;
}

static err_t connection_set_noblock(int32_t fd)
{
    int32_t flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0)
    {
        LOG_ERROR("Cannot get flags from fd %d: errno=%d, %s", fd, errno,
                  strerror(errno));
        return DISFS_ERR_SOCK;
    }
    flags |= O_NONBLOCK;
    int32_t res = fcntl(fd, F_SETFL, flags);
    if (res < 0)
    {
        LOG_ERROR("Cannot set flags for fd %d: errno=%d, %s", fd, errno,
                  strerror(errno));
        return DISFS_ERR_SOCK;
    }
    return DISFS_SUCCESS;
}

static err_t connection_add_event(int32_t epoll, int32_t fd, uint32_t state)
{
    struct epoll_event ev = {0};
    ev.data.fd = fd;
    ev.events = state;
    int32_t res = epoll_ctl(epoll, EPOLL_CTL_ADD, fd, &ev);
    if (res < 0)
    {
        LOG_ERROR("Cannot add event to epoll\n");
        return DISFS_ERR_EPOLL;
    }
    return DISFS_SUCCESS;
}

static err_t connection_accept_client(int32_t epoll,
                                      connection_t connection[static 1])
{
    client_t client = {.active = 1};
    client.fd =
        accept(connection->fd, (struct sockaddr*)&client.addr, &client.len);
    if (client.fd <= 0)
    {
        LOG_ERROR("Cannot accept client: fd=%d, server_fd=%d errno=%d : %s!\n",
                  client.fd, connection->fd, errno, strerror(errno));
        return DISFS_ERR_SOCK;
    }
    LOG_TRACE("Accepted new client: fd=%d\n", client.fd);
    for (int32_t i = 0; i < MAX_NEIGHBOURS; i++)
    {
        if (!connection->clients[i].active)
        {
            connection->clients[i] = client;
            connection_add_event(epoll, client.fd, EPOLLIN);
            return DISFS_SUCCESS;
        }
    }
    LOG_ERROR("Threshhold of active neighbours is reached!\n");
    /* TODO: In this context of reaching max neighbours, server should send info
       to client informing that should find another peer connection */

    return DISFS_ERR_MAX_PEER;
}

static err_t connection_read(client_t client[static 1])
{
    char buffer[1024] = {};
    ssize_t readed = read(client->fd, buffer, sizeof(buffer) - 1);
    LOG_INFO("Readed from %d, size %d, buffer = %s\n", client->fd, readed,
             buffer);
    if (readed <= 0)
    {
        LOG_WARNING("Readed 0 or less bytes from client %d, client will be "
                    "disconnected\n",
                    client->fd);
        return DISFS_ERR_READED;
    }
    return DISFS_SUCCESS;
}

static err_t connection_handle_events(int32_t epoll, struct epoll_event* events,
                                      int32_t events_count,
                                      connection_t connection[static 1])
{
    for (int32_t i = 0; i < events_count; i++)
    {
        int32_t fd = events[i].data.fd;
        if (fd == connection->fd)
        {
            connection_accept_client(epoll, connection);
        }
        else if (fd == connection->udp_fd)
        {
            /*
               Handle broadcasted UDP packet to get new client, after receiving
               this packet, we should initiate connection with following client.

             */
            char buffer[1024] = {};
            struct sockaddr_in src_addr;
            socklen_t addrlen = sizeof(src_addr);

            int n = recvfrom(fd, buffer, sizeof(buffer) - 1, 0,
                             (struct sockaddr*)&src_addr, &addrlen);
            char ip[INET_ADDRSTRLEN] = {};
            inet_ntop(AF_INET, &(src_addr.sin_addr), ip, sizeof(ip));
            LOG_TRACE("Received udp packet: fd=%d, ip=%s, port=%d, buffor=%s\n",
                      fd, ip, ntohs(src_addr.sin_port), buffer);
        }
        else
        {
            client_t* client = NULL;
            for (int32_t j = 0; j < MAX_NEIGHBOURS; j++)
            {
                if (connection->clients[j].fd == fd &&
                    connection->clients[j].active)
                {
                    client = &connection->clients[j];
                }
            }
            if (events[i].events & EPOLLIN)
            {
                if (connection_read(client) != DISFS_SUCCESS)
                {
                    client->active = 0;
                    close(client->fd);
                }
            }
        }
    }
    return DISFS_SUCCESS;
}
