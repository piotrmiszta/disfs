/*
 * Copyright (c) 2025 Piotr Miszta
 * SPDX-License-Identifier: MIT
 */

// clang-foramt off
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>

#include <cmocka.h>
// clang-format on
#include "err_codes.h"
#include "time.h"
#include "udp_discovery.h"
#include <stdio.h>

static void udp_create_test(void** state)
{
    struct timespec before;
    struct timespec after;
    UDP_packet packet = {0};
    timespec_get(&before, TIME_UTC);
    assert_int_equal(udp_discovery_packet_create(&packet, 8080, "TEST", 4),
                     DISFS_SUCCESS);
    timespec_get(&after, TIME_UTC);

    assert_int_equal(packet.tcp_port, 8080);
    printf("%d %d %d\n", before.tv_nsec, after.tv_nsec,
           packet.timestamp.tv_nsec);
    printf("%d %d %d\n", before.tv_sec, after.tv_sec, packet.timestamp.tv_sec);

    assert_in_range(packet.timestamp.tv_sec, before.tv_sec, after.tv_sec);
    if (packet.timestamp.tv_sec == after.tv_sec)
    {
        if (packet.timestamp.tv_sec == before.tv_sec)
        {
            assert_in_range(packet.timestamp.tv_nsec, before.tv_nsec,
                            after.tv_nsec);
        }
        else
        {
            assert_in_range(packet.timestamp.tv_nsec, 0, after.tv_nsec);
        }
    }
    else
    {
        if (packet.timestamp.tv_sec == before.tv_sec)
        {
            assert_in_range(packet.timestamp.tv_nsec, before.tv_nsec,
                            UINT32_MAX);
        }
    }
    assert_int_equal(packet.magic_number, UDP_DISCOVERY_PACKET_MAGIC_NUMBER);
    assert_int_equal(packet.protocol_version, UDP_DISCOVERY_PROTOCOL_VERSION);
    assert_int_equal(packet.hostname_len, 4);
    assert_string_equal(packet.hostname, "TEST");
}

static void udp_serialize_deserialize_test(void** state)
{
    UDP_packet packet = {0};
    assert_int_equal(udp_discovery_packet_create(&packet, 8080, "TEST", 4),
                     DISFS_SUCCESS);
    char serialized[44];
    udp_discovery_packet_serialize(&packet, serialized, 44);
    UDP_packet packet2 = {0};
    udp_discovery_packet_deserialize(&packet2, serialized, 44);
    assert_int_equal(packet2.tcp_port, packet.tcp_port);
    assert_int_equal(packet2.magic_number, packet.magic_number);
    assert_int_equal(packet2.protocol_version, packet.protocol_version);
    assert_int_equal(packet2.timestamp.tv_nsec, packet.timestamp.tv_nsec);
    assert_int_equal(packet2.timestamp.tv_sec, packet.timestamp.tv_sec);
    assert_int_equal(packet2.hostname_len, packet.hostname_len);
    assert_string_equal(packet.hostname, packet2.hostname);
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(udp_create_test),
        cmocka_unit_test(udp_serialize_deserialize_test),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
