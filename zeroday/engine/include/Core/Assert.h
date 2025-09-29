//
// Created by pointerlost on 9/19/25.
//
#pragma once
#include "Logger.h"
#include <format>

// (Platform specific) Debug break
#if defined(_WIN32)
    #define DEBUG_BREAK() __debugbreak()
#elif defined(__linux__) || defined(__APPLE__)
    #include <csignal>
    #define DEBUG_BREAK() raise(SIGTRAP)
#else
    #define DEBUG_BREAK() std::abort()
#endif

// Enable/Disable assertions
#ifdef NDEBUG
    #define ZD_ASSERT(...) ((void)0)
    #define ZD_WARN(...)   ((void)0)
#else
    #define ZD_ASSERT(...) ZD_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__, ZD_INTERNAL_ASSERT_FMT, ZD_INTERNAL_ASSERT_IMPL)(__VA_ARGS__)
    #define ZD_WARN(...)   ZD_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__, ZD_INTERNAL_WARN_FMT, ZD_INTERNAL_WARN)(__VA_ARGS__)
#endif

namespace Zeroday {

    inline void AssertionFailure(const std::string& condition, const std::string& message,
                                const std::string& file, int line) {
        std::string errorMsg = std::format("Assertion failed: {} | File: {}:{} | {}",
                                            condition, file, line, message);
        Error(errorMsg);
        DEBUG_BREAK();
    }

    inline void AssertionWarning(const std::string& condition, const std::string& message,
                                const std::string& file, int line) {
        std::string warnMsg = std::format("Assertion warning: {} | File: {}:{} | {}",
            condition, file, line, message);
        Warn(warnMsg);
    }

}

// ---------------------------------------------------------

// Internal macro helpers for overloading
#define ZD_INTERNAL_ASSERT_IMPL(check, msg) \
    do { \
        if (!(check)) { \
            Zeroday::AssertionFailure(#check, msg, __FILE__, __LINE__); \
        } \
    } while(0)

#define ZD_INTERNAL_ASSERT_FMT(check, fmt, ...) \
    do { \
        if (!(check)) { \
            Zeroday::AssertionFailure(#check, std::format(fmt, __VA_ARGS__), __FILE__, __LINE__); \
        } \
    } while(0)

#define ZD_INTERNAL_WARN(check, msg) \
    do { \
        if (!(check)) { \
            Zeroday::AssertionWarning(#check, msg, __FILE__, __LINE__); \
        } \
    } while(0)

#define ZD_INTERNAL_WARN_FMT(check, fmt, ...) \
    do { \
        if (!(check)) { \
            Zeroday::AssertionWarning(#check, std::format(fmt, __VA_ARGS__), __FILE__, __LINE__); \
        } \
    } while(0)

// ---------------------------------------------------------

// Helper to choose between single/multiple arguments (without message or with message and so on)
#define ZD_INTERNAL_ASSERT_GET_MACRO(_1,_2,NAME,...) NAME
