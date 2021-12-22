#pragma once

#include "platform.h"

#define VKCASSERTIONS_ENABLED TRUE

#ifdef VKCASSERTIONS_ENABLED
#if _MSC_VER
#include <intrin.h>
#define debugBreak() __debugbreak()
#else
#define debugBreak() __builtin_trap()
#endif

VKCAPI void report_assertion_failure(const char *expression, const char *message, const char *file, i32 line);

#define VKCASSERT(expr) {                                           \
    if(expr) {                                                      \
    } else {                                                        \
        report_assertion_failure(#expr, "", __FILE__, __LINE__);    \
        debugBreak();                                               \
    }                                                               \
}

#define VKCASSERT_MSG(expr, message) {                                  \
    if(expr) {                                                          \
    } else {                                                            \
        report_assertion_failure(#expr, message, __FILE__, __LINE__);   \
        debugBreak();                                                   \
    }                                                                   \
}

#ifdef _DEBUG
#define VKCASSERT_DEBUG(expr) {                                     \
    if(expr) {                                                      \
    } else {                                                        \
        report_assertion_failure(#expr, "", __FILE__, __LINE__);    \
        debugBreak();                                               \
    }                                                               \
}
#else
#define VKCASSERT_DEBUG(expr) //Do nothing.
#endif

#else
#define VKCASSERT(expr) //Do nothing.
#define VKCASSERT_MSG(expr, message) //Do nothing.
#define VKCASSERT_DEBUG(expr) //Do nothing.
#endif
