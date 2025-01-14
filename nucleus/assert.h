/**
 * (c) 2015 Alexandro Sanchez Bach. All rights reserved.
 * Released under GPL v2 license. Read LICENSE for more details.
 */

#pragma once

#include "nucleus/macro.h"

#include <cassert>

// Trigger an exception if expression is false
#define assert_true(...) \
    __NUCLEUS_MACRO_DISPATCH(assert_true, __VA_ARGS__)(__VA_ARGS__)
#define assert_true1(expr) assert(expr)
#define assert_true2(expr, message) assert((expr) && (message))

// Trigger an exception if expression is true
#define assert_false(...) \
    __NUCLEUS_MACRO_DISPATCH(assert_false, __VA_ARGS__)(__VA_ARGS__)
#define assert_false1(expr) assert(!(expr))
#define assert_false2(expr, message) assert(!(expr) && (message))

// Trigger an exception if expression is zero
#define assert_zero(...) \
    __NUCLEUS_MACRO_DISPATCH(assert_zero, __VA_ARGS__)(__VA_ARGS__)
#define assert_zero1(expr) assert(expr)
#define assert_zero2(expr, message) assert(((expr) == 0) && (message))

// Trigger an exception if expression is non-zero
#define assert_nonzero(...) \
    __NUCLEUS_MACRO_DISPATCH(assert_true, __VA_ARGS__)(__VA_ARGS__)
#define assert_nonzero1(expr) assert_nonzero(expr)
#define assert_nonzero2(expr, message) assert(((expr) != 0) && (message))

// Trigger an exception if expression is a nullptr
#define assert_null(...) \
    __NUCLEUS_MACRO_DISPATCH(assert_null, __VA_ARGS__)(__VA_ARGS__)
#define assert_null1(expr) assert(expr)
#define assert_null2(expr, message) assert(((expr) == nullptr) && (message))

// Trigger an exception if expression is not a nullptr
#define assert_nonnull(...) \
    __NUCLEUS_MACRO_DISPATCH(assert_nonnull, __VA_ARGS__)(__VA_ARGS__)
#define assert_nonnull1(expr) assert(expr)
#define assert_nonnull2(expr, message) assert(((expr) != nullptr) && (message))

// Trigger an exception
#define assert_always(...) \
    __NUCLEUS_MACRO_DISPATCH(assert_always, __VA_ARGS__)(__VA_ARGS__)
#define assert_always0(...) assert(0)
#define assert_always1(message) assert(0 && (message))
