#pragma once

#include <memory>

#include "Types.h"

// TODO(patrik): Try to fix the formatting 'IndentPPDirectives'
#ifdef _WIN32
/* Windows x64/x86 */
#    ifdef _WIN64
/* Windows x64  */
#        define SK_PLATFORM_WINDOWS
#    else
/* Windows x86 */
#        error "x86 Builds are not supported!"
#    endif
#endif

#define SK_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

#define BIT(x) (1 << x)

#define SK_CORE_ASSERT(x, ...)                                                 \
    {                                                                          \
        while (!(x))                                                           \
        {                                                                      \
            SK_CORE_ERROR("------------------------------------");             \
            SK_CORE_ERROR("  Core Assertion Failed");                          \
            SK_CORE_ERROR("  Message:");                                       \
            SK_CORE_ERROR("  " __VA_ARGS__);                                   \
            SK_CORE_ERROR("------------------------------------");             \
            __debugbreak();                                                    \
        }                                                                      \
    }
#define SK_APP_ASSERT(x, ...)                                                  \
    {                                                                          \
        while (!(x))                                                           \
        {                                                                      \
            SK_APP_ERROR("------------------------------------");              \
            SK_APP_ERROR("  App Assertion Failed");                            \
            SK_APP_ERROR("  Message:");                                        \
            SK_APP_ERROR("  " __VA_ARGS__);                                    \
            SK_APP_ERROR("------------------------------------");              \
            __debugbreak();                                                    \
        }                                                                      \
    }

namespace Seek
{
    template <typename T> using Scope = std::unique_ptr<T>;
    template <typename T, typename... Args>
    constexpr Scope<T> CreateScope(Args&&... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template <typename T> using Ref = std::shared_ptr<T>;
    template <typename T, typename... Args>
    constexpr Ref<T> CreateRef(Args&&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}