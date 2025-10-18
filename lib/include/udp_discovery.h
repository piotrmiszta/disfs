/*
 * Copyright (c) 2025 Piotr Miszta
 * SPDX-License-Identifier: MIT
 */

#ifndef DISFS_UDP_DISCOVERY_H_
#define DISFS_UDP_DISCOVERY_H_

#define UDP_DISCOVERY_HOSTNAME_MAX_LEN 20
#define UDP_DISCOVERY_PACKET_MAGIC_NUMBER 0xAE
#define UDP_DISCOVERY_PROTOCOL_VERSION 0x01
#include "err_codes.h"
#include <stdint.h>
#include <time.h>

typedef struct UDP_packet
{
    int32_t tcp_port;
    int32_t protocol_version;
    struct timespec timestamp;
    uint32_t magic_number;
    int32_t hostname_len;
    char hostname[UDP_DISCOVERY_HOSTNAME_MAX_LEN];
} UDP_packet;

err_t udp_discovery_packet_create(UDP_packet packet[static 1], int32_t tcp_port,
                                  const char* hostname,
                                  uint32_t hostname_length);
err_t udp_discovery_packet_serialize(UDP_packet packet[static 1], char* buffer,
                                     uint32_t buffer_len);
err_t udp_discovery_packet_deserialize(UDP_packet packet[static 1],
                                       char* buffer, uint32_t buffer_len);

#endif
