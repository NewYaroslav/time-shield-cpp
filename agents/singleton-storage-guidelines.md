# Singleton / Service Storage Playbook

Use this document when you are changing singleton-style service storage in header-oriented C++ code.

English is the canonical version.

Russian localization:
- `agents/singleton-storage-guidelines-RU.md`

Canonical developer doc:
- `../docs/singleton-storage-guidelines.md`
- `../docs/singleton-storage-guidelines-RU.md`

## Choose by language standard

- **C++17 and newer**
  - prefer `static inline` storage inside the class
  - use this when the service is truly header-owned
- **C++11/14**
  - do not pretend the storage is fully header-only
  - define storage in exactly one owning TU through a macro

## Preferred C++17+ pattern

```cpp
class Service {
public:
    static Service& instance() noexcept {
        return s_instance;
    }

private:
    Service() = default;
    static inline Service s_instance{};
};
```

## Required C++11/14 pattern

```cpp
#pragma once

class Service {
public:
    static Service& instance() noexcept;

private:
    Service() = default;
#if __cplusplus >= 201703L
    static inline Service s_instance{};
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

Define the macro in exactly one `.cpp` file:

```cpp
#define SERVICE_DEFINE_STORAGE
#include "Service.hpp"
```

All other TUs include the header without the macro.

## Keep ownership explicit

- Put raw storage in `detail`.
- Use one-TU ownership for C++11/14.
- If many services need this pattern, prefer one dedicated TU such as `project_singletons.cpp`.

## Macro naming

Use one of these patterns:

- `FOO_DEFINE_STORAGE`
- `FOO_IMPLEMENTATION`

Exactly one TU must define the chosen macro.

## Review checklist

Before finalizing:

- the target standard is clear
- C++17+ uses `static inline` storage where appropriate
- C++11/14 storage is owned by exactly one TU
- raw storage belongs in `detail`, outside the public API
- macro naming is consistent
- missing or duplicate storage definitions are not possible in the chosen build path
