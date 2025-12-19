# Coding Guidelines

## Variable Naming
- Prefix `m_` must always be used for class fields (e.g., `m_event_hub`, `m_task_manager`).
- Prefixes `p_` and `str_` are optional when a function or method has more than five variables or arguments of different types. If there are fewer than five, they may be omitted.
- Boolean variables must start with `is`, `has`, `use`, `enable` or with `m_is_`, `m_has_` and so on for class fields (e.g., `is_connected`, `m_is_active`).
- Do not use prefixes `b_`, `n_`, `f_`.

## Doxygen Comments
- All comments and Doxygen must be in English.
- Use `/// \brief` before functions and classes.
- Do not start descriptions with `The`.

## File Names
- If a file contains only one class, use CamelCase (e.g., `TradeManager.hpp`).
- If a file contains multiple classes, utilities, or helper structures, use snake_case (e.g., `trade_utils.hpp`, `market_event_listener.hpp`).

## Entity Names
- Names of classes, structures, and enumerations use CamelCase.
- Method names use snake_case.

## Method Names
- Methods are named in snake_case.
- Getter methods may omit the `get_` prefix when they:
  - Do not perform complex computations and simply return a reference or value.
  - Provide access to an internal object rather than calculating a new value.
  - Behave like a property of the object, similar to `size()` or `empty()` in the STL.
- Use the `get_` prefix when:
  - The method computes a value before returning it.
  - Without `get_`, the method name could be misleading.

## Commit Messages

- Use Conventional Commits format: `type(scope): summary`.
- The header must be in English.
- Include a body that describes the change.

## Changes

- Keep diffs minimal and focused.
- Do not refactor or apply style changes beyond the lines you directly touch.

## Header-only singleton/service storage rule (C++11–C++20)

Use this rule to avoid ODR issues while keeping a single instance per program.

### C++17 and newer
Prefer `static inline` storage inside the class:

```cpp
class Service {
public:
    static Service& instance() noexcept {
        return s_instance;
    }

private:
    Service() = default;
    static inline Service s_instance{}; // single instance (C++17+)
};
```

### C++11/14 (header-only is not possible without one TU)
You must define storage in exactly one translation unit (TU) using a macro.

Header (`Service.hpp`):

```cpp
#pragma once

class Service {
public:
    static Service& instance() noexcept;

private:
    Service() = default;
#if __cplusplus >= 201703L
    static inline Service s_instance{}; // single instance (C++17+)
#endif
};

#if __cplusplus >= 201703L

inline Service& Service::instance() noexcept {
    return s_instance;
}

#else

namespace detail {
#   if defined(SERVICE_DEFINE_STORAGE)
        Service g_service;
#   else
        extern Service g_service;
#   endif
}

inline Service& Service::instance() noexcept {
    return detail::g_service;
}

#endif
```

Usage (C++11/14): define the macro in exactly one `.cpp` file (usually `main.cpp`):

```cpp
#define SERVICE_DEFINE_STORAGE
#include "Service.hpp"
```

All other `.cpp` files should include without the macro:

```cpp
#include "Service.hpp"
```

Notes:
- The macro must be defined in exactly one TU.
- If it is not defined anywhere, you will get an undefined reference error.
- If it is defined in multiple TUs, you will get multiple definition errors.
- In C++17+, the macro is unnecessary (but harmless).
- Use `detail` to keep raw storage out of public API.
- If many services exist in C++11/14, prefer a single TU like `project_singletons.cpp` that defines all `*_DEFINE_STORAGE` macros.

Naming convention for macros:
- `FOO_DEFINE_STORAGE` (or `FOO_IMPLEMENTATION`) — exactly one TU defines it.
