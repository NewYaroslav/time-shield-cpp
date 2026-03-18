# Playbook по singleton / service storage

Используйте этот документ, когда меняете хранение singleton-подобных сервисов в header-oriented C++ коде.

Канонической версией является английская:

- `agents/singleton-storage-guidelines.md`

Canonical developer doc:
- `../docs/singleton-storage-guidelines.md`
- `../docs/singleton-storage-guidelines-RU.md`

## Выбирайте по стандарту языка

- **C++17 и новее**
  - предпочитайте `static inline` storage внутри класса
  - используйте этот вариант, когда сервис действительно header-owned
- **C++11/14**
  - не считайте такое storage полностью header-only
  - определяйте storage ровно в одном owning TU через макрос

## Предпочтительный паттерн для C++17+

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

## Обязательный паттерн для C++11/14

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

Определяйте макрос ровно в одном `.cpp` файле:

```cpp
#define SERVICE_DEFINE_STORAGE
#include "Service.hpp"
```

Во всех остальных TU заголовок подключается без макроса.

## Делайте ownership явным

- Размещайте raw storage в `detail`.
- Для C++11/14 используйте one-TU ownership.
- Если таких сервисов много, предпочтителен один выделенный TU вроде `project_singletons.cpp`.

## Именование макросов

Используйте один из этих паттернов:

- `FOO_DEFINE_STORAGE`
- `FOO_IMPLEMENTATION`

Выбранный макрос должен быть определён ровно в одном TU.

## Checklist для ревью

Перед финализацией проверьте:

- целевой стандарт понятен
- в C++17+ там, где уместно, используется `static inline` storage
- в C++11/14 storage принадлежит ровно одному TU
- raw storage остаётся вне публичного API и живёт в `detail`
- именование макросов последовательно
- выбранный путь сборки исключает отсутствующее или дублирующееся определение storage
