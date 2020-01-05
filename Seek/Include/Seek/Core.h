#pragma once

#include "Types.h"

#define SK_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

#define BIT(x) (1 << x)

#define SK_CORE_ASSERT(x, ...) { if(!(x)) { SK_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define SK_APP_ASSERT(x, ...) { if(!(x)) { SK_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }