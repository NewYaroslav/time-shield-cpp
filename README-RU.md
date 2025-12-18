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
- **Тип `DateTime`** — обёртка, хранящая UTC миллисекунды и фиксированное смещение, поддерживает ISO8601, локальные/UTC компоненты и арифметику.
- **ISO week date** — конвертация, форматирование и парсинг ISO 8601 для недельного счёта.
- **Астрономические утилиты** — расчёт Julian Date/MJD/JDN и оценка лунной фазы/возраста по Unix‑времени.
- **Утилиты** — получение текущих меток времени, вычисления начала/конца периодов, работа с частями секунды.
- **Преобразование часовых поясов** — функции для CET/EET в GMT.
- **NTP‑клиент и пул** — одиночные запросы и конфигурируемый пул/раннер/сервис с возможностью офлайн‑тестов (Windows и Unix).
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

### Класс DateTime

`DateTime` хранит UTC миллисекунды и фиксированный offset, что позволяет
сохранять смещение при форматировании и получать локальные компоненты.

```cpp
#include <time_shield.hpp>

using namespace time_shield;

DateTime dt = DateTime::parse_iso8601("2025-12-16T10:20:30.123+02:30");
std::string local = dt.to_iso8601();        // сохраняет +02:30
std::string utc = dt.to_iso8601_utc();      // выводит Z
DateTime tomorrow = dt.add_days(1).start_of_day();
int hour_local = dt.hour();                 // локальный час с учётом offset
int hour_utc = dt.utc_hour();               // час в UTC
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
MoonPhaseSineCosine signal = moon_phase_sincos(fts()); // sin/cos фазового угла без скачка в 0/1
MoonQuarterInstants quarters = moon_quarters(fts());   // ближайшие четверти (Unix секунды в double)
bool is_near_new = is_new_moon_window(fts());  // попадание в окно новолуния +/-12ч
```

### Геоцентрический калькулятор фаз Луны

`MoonPhaseCalculator` (`time_shield::astronomy::MoonPhase`) выдаёт расширенный набор показателей (освещённость, угловые диаметры, расстояние, фазовый угол), sin/cos для непрерывного фазового сигнала, моменты четвертей и проверки «окон» вокруг фаз. Текущая математика геоцентрическая: положения Солнца/Луны рассчитываются без топоцентрических поправок. Поэтому освещённость и фазовый угол — «глобальные» в данный момент времени, а локально отличается:

- дата/время из-за часового пояса,
- ориентация освещённой части (в южном полушарии картинка отражена),
- наблюдаемость (первый серп, высота над горизонтом, атмосфера).

> Текущая математика — геоцентрическая (относительно центра Земли), без топоцентрических поправок/параллакса. Освещённость/фазовый угол глобальны для Земли как целого. По месту реально меняются:
> - локальная дата/время (часовой пояс),
> - ориентация освещённой части (в северном/южном полушарии «картинка» перевёрнута),
> - видимость (первый серп/наблюдаемость) — уже про атмосферу/высоту над горизонтом и т.п.

```cpp
#include <time_shield/MoonPhase.hpp>

using namespace time_shield;

MoonPhaseCalculator calculator{};
const double ts = 1704067200.0; // 2024-01-01T00:00:00Z
MoonPhaseResult res = calculator.compute(ts);
MoonPhase::quarters_unix_s_t quarters = calculator.quarter_times_unix(ts); // Unix секунды (double)
MoonQuarterInstants around = moon_quarters(ts);
MoonPhaseSineCosine signal = moon_phase_sincos(ts);
bool is_new = calculator.is_new_moon_window(ts); // по умолчанию окно +/-12ч
```

### Конвертация часовых поясов

```cpp
#include <time_shield.hpp>

ts_t cet = to_ts(2024, Month::JUN, 21, 12, 0, 0);
ts_t gmt = cet_to_gmt(cet);
```

### NTP‑клиент, пул и сервис времени

```cpp
#include <time_shield/ntp_client_pool.hpp>
#include <time_shield/ntp_time_service.hpp>

using namespace time_shield;

NtpClientPool pool;
pool.set_default_servers();
pool.measure();
int64_t pool_offset = pool.offset_us();

// Фоновый runner + ленивый сервис:
auto& ntp = NtpTimeService::instance();
ntp.init(std::chrono::seconds(30)); // или time_shield::ntp::init(...)
int64_t utc_ms = ntp.utc_time_ms();
ntp.shutdown();
```

## Документация

Полное описание API и дополнительные примеры доступны по адресу: <https://newyaroslav.github.io/time-shield-cpp/>

## Лицензия

Проект распространяется по лицензии [MIT](LICENSE).
