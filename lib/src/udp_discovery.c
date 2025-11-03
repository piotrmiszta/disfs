/*
 * Copyright (c) 2025 Piotr Miszta
 * SPDX-License-Identifier: MIT
 */

#include "udp_discovery.h"
#include "err_codes.h"
#include "logger.h"
#include <stdlib.h>
#include <time.h>

#define LEN 44

err_t udp_discovery_packet_create(UDP_packet packet[static 1], int32_t tcp_port,
                                  const char* hostname,
                                  uint32_t hostname_length)
{
    if (hostname_length > UDP_DISCOVERY_HOSTNAME_MAX_LEN)
    {
        LOG_ERROR("Hostname is longer than maximum allowed hostname\n");
        return DISFS_ERR_INVALID_ARG;
    }
    packet->tcp_port = tcp_port;
    packet->magic_number = UDP_DISCOVERY_PACKET_MAGIC_NUMBER;
    packet->protocol_version = UDP_DISCOVERY_PROTOCOL_VERSION;
    packet->hostname_len = hostname_length;
    memcpy(packet->hostname, hostname, hostname_length);
    timespec_get(&packet->timestamp, TIME_UTC);
    return DISFS_SUCCESS;
}

err_t udp_discovery_packet_serialize(UDP_packet packet[static 1], char* buffer,
                                     int64_t buffer_len)
{
    ASSERT(buffer_len >= LEN,
           "Invalid size of buffer for udp packet serialize\n");
    if (packet->hostname_len > UDP_DISCOVERY_HOSTNAME_MAX_LEN)
    {
        LOG_ERROR("Hostname is longer than maximum allowed hostname\n");
        return DISFS_ERR_INVALID_ARG;
    }
    memset(buffer, 0, LEN);
    size_t int_len = sizeof(int);
    memcpy(buffer, &packet->tcp_port, int_len);
    buffer += int_len;
    memcpy(buffer, &packet->magic_number, int_len);
    buffer += int_len;
    memcpy(buffer, &packet->protocol_version, int_len);
    buffer += int_len;
    memcpy(buffer, &packet->timestamp.tv_nsec, int_len);
    buffer += int_len;
    memcpy(buffer, &packet->timestamp.tv_sec, int_len);
    buffer += int_len;
    memcpy(buffer, &packet->hostname_len, int_len);
    buffer += int_len;
    memcpy(buffer, &packet->hostname, (size_t)packet->hostname_len);
    return DISFS_SUCCESS;
}

err_t udp_discovery_packet_deserialize(UDP_packet packet[static 1],
                                       char* buffer, int64_t buffer_len)
{
    ASSERT(buffer_len >= LEN,
           "Invalid size of buffer for udp packet serialize\n");
    size_t int_len = sizeof(int);
    memcpy(&packet->tcp_port, buffer, int_len);
    buffer += int_len;
    memcpy(&packet->magic_number, buffer, int_len);
    buffer += int_len;
    memcpy(&packet->protocol_version, buffer, int_len);
    buffer += int_len;
    memcpy(&packet->timestamp.tv_nsec, buffer, int_len);
    buffer += int_len;
    memcpy(&packet->timestamp.tv_sec, buffer, int_len);
    buffer += int_len;
    memcpy(&packet->hostname_len, buffer, int_len);
    buffer += int_len;
    memset(packet->hostname, 0, UDP_DISCOVERY_HOSTNAME_MAX_LEN);
    memcpy(packet->hostname, buffer, (size_t)packet->hostname_len);
    return DISFS_SUCCESS;
}
