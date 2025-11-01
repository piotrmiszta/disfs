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

static void simple_test(void** state) { assert_int_equal(1, 1); }

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(simple_test),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
