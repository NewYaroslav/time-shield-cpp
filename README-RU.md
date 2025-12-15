# Time Shield

<img src="docs/logo-1280x640.png" alt="Логотип" width="600"/>

![MIT License](https://img.shields.io/badge/license-MIT-green.svg)
![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20MQL5-blue)
![C++ Standard](https://img.shields.io/badge/C++-11--17-orange)
![CI Windows](https://img.shields.io/github/actions/workflow/status/newyaroslav/time-shield-cpp/ci.yml?branch=main&label=Windows&logo=windows)
![CI Linux](https://img.shields.io/github/actions/workflow/status/newyaroslav/time-shield-cpp/ci.yml?branch=main&label=Linux&logo=linux)
![CI macOS](https://img.shields.io/github/actions/workflow/status/newyaroslav/time-shield-cpp/ci.yml?branch=main&label=macOS&logo=apple)

**Time Shield** — это header-only C++-библиотека для работы со временем. Она включает функции для конвертации временных значений, форматирования дат и множество утилит для задач с таймстампами.
> Названа в честь «временного щита» Хомуры Акэми.

## Зачем Time Shield?

**Time Shield** создавался как практичный инструмент для работы с временем в C++, ориентированный на прикладные и инженерные задачи. В отличие от стандартной `std::chrono` или более академичных решений вроде `HowardHinnant/date`, библиотека:

- использует простые типы (`int64_t`, `double`) для представления времени (`ts_t`, `fts_t`) — их легко логировать, сериализовать и передавать через JSON, RPC и базы данных; без перегруженных классов `std::chrono`;
- поддерживает **разные временные представления** — Unix‑время, дробные секунды, милли- и микросекунды, OLE Automation (Excel), джулианские даты;
- включает **утилиты для округления, форматирования, парсинга ISO8601**, работы с частями времени и вычисления границ периодов;
- имеет **расширяемую архитектуру** — новые форматы (Julian, OLE, UTC-offset) добавляются как отдельные типы и модули;
- **работает даже в ограниченных средах**, таких как MQL5/MetaTrader — не требует исключений, STL-контейнеров или динамической памяти;
- поставляется как **header-only** — подключается одной строкой, без сборки и внешних зависимостей;
- использует только **стандартные заголовки STL и системные API**; платформенные модули (например, сокетный `NtpClient`) изолированы и не мешают кроссплатформенной сборке.

## Возможности

- **Проверка дат** — валидация дат с учётом високосных лет и выходных.
- **Форматирование времени** — преобразование таймстампов в строки по стандартным и пользовательским шаблонам.
- **Конвертации** — перевод между секундными, миллисекундными и плавающими представлениями времени, структуры `DateTimeStruct`, OLE Automation (Excel) и временные зоны.
- **Астрономические утилиты** — расчёт Julian Date/MJD/JDN и оценка лунной фазы/возраста по Unix‑времени.
- **Утилиты** — получение текущих меток времени, вычисления начала/конца периодов, работа с частями секунды.
- **Преобразование часовых поясов** — функции для CET/EET в GMT.
- **NTP‑клиент** — получение точного времени по сети (Windows и Unix).
- **Поддержка MQL5** — адаптированные заголовки в каталоге `MQL5` позволяют использовать библиотеку в MetaTrader.
- Совместимость с `C++11` – `C++17`.

## Конфигурация

Компиляционные флаги в `time_shield/config.hpp` позволяют адаптировать библиотеку под платформу и отключать необязательные модули:

- `TIME_SHIELD_PLATFORM_WINDOWS` / `TIME_SHIELD_PLATFORM_UNIX` — определение целевой платформы.
- `TIME_SHIELD_HAS_WINSOCK` — наличие WinSock API.
- `TIME_SHIELD_ENABLE_NTP_CLIENT` — включает модуль `NtpClient` (по умолчанию `1` на поддерживаемых платформах).

Все заголовки библиотеки используют пространство имён `time_shield`. Для доступа к API можно писать `time_shield::` или подключать `using namespace time_shield;`.

> Часть функций зависит от системных API и может быть ограничена платформой (например, получение realtime через `QueryPerformanceCounter` под Windows).

## Установка и настройка

Библиотека поставляется в виде заголовков. Для подключения достаточно добавить путь `include/time_shield` в проект и подключить основной файл:

```cpp
#include <time_shield.hpp>
```

Примеры можно собрать скриптом `build-examples.bat`. Для установки файлов MQL5 предусмотрен `install_mql5.bat`.

## Примеры использования

Ниже приведены небольшие примеры из разделов библиотеки.

### Получение и конвертация времени

```cpp
#include <time_shield.hpp>

using namespace time_shield;

ts_t now = ts();                 // секунды с эпохи
fts_t now_f = fts();             // время в секундах с дробной частью
int ms_part = ms_of_sec(now_f);  // миллисекундная часть
```

### Форматирование дат

```cpp
#include <time_shield.hpp>

std::string iso = to_iso8601(now);          // 2024-06-21T12:00:00
std::string custom = to_string("%Y-%m-%d %H:%M:%S", now);
std::string mql5 = to_mql5_date_time(now);  // 2024.06.21 12:00:00
std::string filename = to_windows_filename(now);
```

### Парсинг ISO8601

```cpp
#include <time_shield.hpp>

DateTimeStruct dt;
TimeZoneStruct tz;
if (parse_iso8601("2024-11-25T14:30:00-05:30", dt, tz)) {
    ts_t ts_val = to_timestamp(dt) + to_offset(tz);
}
```

### Преобразование дат OLE Automation (OA)

Преобразования OA совместимы с Excel/COM (базовая дата 1899-12-30), выполняются в UTC и сохраняют округление к нулю, характерное для серийных OA дат.

```cpp
#include <time_shield/ole_automation_conversions.hpp>

using namespace time_shield;

oadate_t oa = ts_to_oadate(1714608000);                     // 2024-05-02 00:00:00Z
ts_t ts_from_oa = oadate_to_ts(oa);                         // преобразование обратно в Unix-время
oadate_t from_parts = to_oadate(2024, Month::MAY, 2, 12, 0); // 2024-05-02 12:00:00Z
```

### Джулианские даты и лунные вычисления

Астрономические хелперы ориентированы на аналитические оценки (JD, MJD, JDN, фазу и возраст Луны), а не на высокоточные эфемериды.

```cpp
#include <time_shield/astronomy_conversions.hpp>

using namespace time_shield;

jd_t jd = ts_to_jd(1714608000);               // Julian Date для переданного таймстампа
mjd_t mjd = ts_to_mjd(1714608000);             // Modified Julian Date
jdn_t jdn = gregorian_to_jdn(2, 5, 2024);      // Julian Day Number (целое значение)
double phase = moon_phase(fts());              // фаза Луны [0..1)
double age_days = moon_age_days(fts());        // примерный возраст Луны в днях
```

### Конвертация часовых поясов

```cpp
#include <time_shield.hpp>

ts_t cet = to_ts(2024, Month::JUN, 21, 12, 0, 0);
ts_t gmt = cet_to_gmt(cet);
```

### NTP‑клиент (Windows)

```cpp
#include <time_shield.hpp>

NtpClient client;
if (client.query()) {
    int64_t offset = client.get_offset_us();
    int64_t utc_ms = client.get_utc_time_ms();
}
```

## Документация

Полное описание API и дополнительные примеры доступны по адресу: <https://newyaroslav.github.io/time-shield-cpp/>

## Лицензия

Проект распространяется по лицензии [MIT](LICENSE).

