#ifndef KERNEL_ASSERT_H_
#define KERNEL_ASSERT_H_

#include <stdint.h>

// Runtime assert
// There are 3 types of assert level. Here is table when use which one:
//  TYPE    PLACES      PERFORMANCE IMPACT
//
//  HIGH    ANY         HEAVY
//  MID     IMPORTANT   SOME
//  LOW     CRITICAL    NONE
//
// To change assert level go to meson.build

// NOLINTBEGIN

#ifdef DEBUG

void assert_check(uint8_t condition_result, char *message);

#define ASSERT_BASE(condition, message) assert_check((condition), (message))

#if ASSERT_LEVEL >= 1
#define ASSERT_LOW(condition, message) ASSERT_BASE(condition, message)
#else
#define ASSERT_LOW(condition, message)
#endif

#if ASSERT_LEVEL >= 2
#define ASSERT_MID(condition, message) ASSERT_BASE(condition, message)
#else
#define ASSERT_MID(condition, message)
#endif

#if ASSERT_LEVEL >= 3
#define ASSERT_HIGH(condition, message) ASSERT_BASE(condition, message)
#else
#define ASSERT_HIGH(condition, message)
#endif

#define ASSERT(level, condition, message) ASSERT_##level(condition, message)

#else

#define ASSERT(priority, condition, message)

#endif

// NOLINTEND

#endif