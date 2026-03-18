# Header-only Singleton / Service Storage Guidelines

This document defines a reusable policy for singleton-style service storage in header-oriented C++ projects.

The guidance is intentionally generic so it can be copied into other repositories with minimal changes.

## Canonical Language

English is the canonical version of this document.

Russian localization:
- `docs/singleton-storage-guidelines-RU.md`

Agent playbook:
- `agents/singleton-storage-guidelines.md`
- `agents/singleton-storage-guidelines-RU.md`

## Purpose

Use this guidance when deciding:

- how to keep one service instance per program without ODR issues
- when `static inline` storage is sufficient
- when C++11/14 requires one-TU ownership
- how to keep raw storage out of the public API
- how to name storage-definition macros consistently

## Core Rule

Use this policy to avoid ODR issues while keeping a single instance per program.

## C++17 and newer

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

Use this model when:

- the service is truly header-owned
- inline storage is available for the target standard
- there is no need to move emission into a dedicated ownership TU

## C++11/14

Header-only singleton storage is not possible without exactly one owning translation unit (TU).

Define storage in exactly one TU using a macro.

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

Usage (C++11/14): define the macro in exactly one `.cpp` file, usually `main.cpp` or a dedicated ownership TU:

```cpp
#define SERVICE_DEFINE_STORAGE
#include "Service.hpp"
```

All other `.cpp` files should include the header without the macro:

```cpp
#include "Service.hpp"
```

## Ownership Boundaries

Keep raw storage out of the public API by placing it in `detail`.

This is the preferred model when:

- the service must exist once per program
- the library supports C++11/14
- ownership must stay explicit in non-header-only builds

If many services need C++11/14 storage, prefer a single TU such as `project_singletons.cpp` that defines all `*_DEFINE_STORAGE` macros.

## Macro Naming

Use one of these naming patterns for the storage-definition macro:

- `FOO_DEFINE_STORAGE`
- `FOO_IMPLEMENTATION`

Exactly one TU must define the chosen macro.

## Failure Modes

- If the macro is not defined anywhere in C++11/14 builds, you will get an undefined reference error.
- If it is defined in multiple TUs, you will get multiple definition errors.
- In C++17+, the macro is unnecessary, but leaving it defined is harmless if the implementation is written accordingly.

## Practical Checklist

Before finalizing a singleton-style service:

- the project standard level is clear
- C++17+ code uses `static inline` storage when the service is truly header-owned
- C++11/14 code defines storage in exactly one TU
- raw storage stays in `detail`
- macro naming follows a consistent `*_DEFINE_STORAGE` or `*_IMPLEMENTATION` pattern
- failure modes are understood and documented where needed
