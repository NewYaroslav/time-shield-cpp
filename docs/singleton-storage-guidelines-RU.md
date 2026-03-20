# Рекомендации по хранению singleton / service storage в header-only C++ коде

Этот документ задаёт переносимую политику для хранения singleton-подобных сервисов в C++ проектах с заголовочной архитектурой.

Текст намеренно написан в обобщённом виде, чтобы его можно было перенести в другой репозиторий с минимальными правками.

## Канонический язык

Канонической версией этого документа является английская:

- `docs/singleton-storage-guidelines.md`

Agent playbook:
- `agents/singleton-storage-guidelines.md`
- `agents/singleton-storage-guidelines-RU.md`

## Назначение

Используйте эти рекомендации, когда нужно решить:

- как держать один экземпляр сервиса на программу без ODR-проблем
- когда достаточно `static inline` storage
- когда в C++11/14 нужен один owning translation unit
- когда immortal singleton лучше подходит для сервисов, которые должны переживать late teardown
- как убрать raw storage из публичного API
- как последовательно именовать макросы для определения storage

## Базовое правило

Используйте эту политику, чтобы избежать ODR-проблем и при этом сохранить один экземпляр на программу.

## C++17 и новее

Предпочитайте `static inline` storage внутри класса:

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

Этот вариант уместен, когда:

- сервис действительно принадлежит заголовку
- целевой стандарт поддерживает inline storage
- нет необходимости переносить emission в отдельный ownership TU

## C++11/14

Большинство шаблонов хранения singleton в C++11/14 требуют ровно один owning translation unit (TU).

Определяйте storage ровно в одном TU с помощью макроса.

Заголовок (`Service.hpp`):

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

Использование в C++11/14: определите макрос ровно в одном `.cpp` файле, обычно в `main.cpp` или отдельном ownership TU:

```cpp
#define SERVICE_DEFINE_STORAGE
#include "Service.hpp"
```

Во всех остальных `.cpp` файлах подключайте заголовок без макроса:

```cpp
#include "Service.hpp"
```

## Исключение для late-teardown-safe сервисов в C++11/14

Когда сервис должен оставаться доступным во время разрушения статических объектов, допустим и header-only immortal singleton.

Типовая форма:

```cpp
class Service {
public:
    static Service& instance() noexcept {
        static Service* p_instance = create_instance();
        return *p_instance;
    }

private:
    Service() = default;

    static Service* create_instance() noexcept {
        Service* p_instance = new Service();
        std::atexit(&begin_process_shutdown);
        return p_instance;
    }

    static void begin_process_shutdown() noexcept {
        instance().stop_without_restarting();
    }
};
```

Используйте эту модель только если одновременно верны все условия:

- сервис должен переживать проблемы порядка разрушения статических объектов
- утечка одного process-lifetime экземпляра допустима
- shutdown path явно no-throw
- поздние вызовы не должны повторно запускать фоновую работу во время завершения процесса

`NtpTimeService` в этом репозитории использует именно такой immortal singleton, а не старую one-TU macro model.

## Границы ownership

Держите raw storage вне публичного API, размещая его в `detail`.

Эта модель предпочтительна, когда:

- сервис должен существовать ровно один раз на программу
- библиотека поддерживает C++11/14
- ownership должен оставаться явным в non-header-only сборках

Если сервисов много и нужен C++11/14 storage, предпочтителен единый TU вроде `project_singletons.cpp`, который определяет все `*_DEFINE_STORAGE` макросы.

## Именование макросов

Используйте один из этих паттернов для макроса определения storage:

- `FOO_DEFINE_STORAGE`
- `FOO_IMPLEMENTATION`

Выбранный макрос должен быть определён ровно в одном TU.

## Сценарии ошибок

- Если в сборке C++11/14 макрос не определён нигде, будет undefined reference error.
- Если макрос определён в нескольких TU, будут multiple definition errors.
- В C++17+ макрос не нужен, но его наличие безвредно, если реализация написана соответствующим образом.

## Практический checklist

Перед финализацией singleton-подобного сервиса проверьте:

- уровень стандарта проекта понятен
- в C++17+ используется `static inline` storage, когда сервис действительно header-owned
- в C++11/14 storage либо определяется ровно в одном TU, либо осознанно используется immortal singleton для late-teardown-safe поведения
- raw storage размещается в `detail`
- макрос именован последовательно по шаблону `*_DEFINE_STORAGE` или `*_IMPLEMENTATION`
- сценарии ошибок понятны и при необходимости задокументированы
