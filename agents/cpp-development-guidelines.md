# C++ Development Guidelines

## Variable naming

- Class fields must use `m_` prefix (for example `m_event_hub`, `m_task_manager`).
- Prefixes `p_` and `str_` are optional, and only recommended when a function/method has more than five variables or mixed argument types.
- Boolean names must start with `is`, `has`, `use`, `enable`, or member-field forms like `m_is_`, `m_has_`.
- Do not use `b_`, `n_`, `f_` prefixes.

## Doxygen comments

- All comments and Doxygen text must be in English.
- Use `/// \brief` before classes and functions.
- Do not start descriptions with `The`.
- Describe current behavior and contract in declarative present-tense style.
- Avoid migration/comparison wording such as `now`, `no longer`, `remains`, `stays`, `previously`, and `currently` unless the text is explicitly documenting history or migration.
- Do not explain obvious consequences of the current contract unless that wording removes a real ambiguity.

## File naming

- If a file contains one class, use `CamelCase` file names (for example `TradeManager.hpp`).
- If a file contains multiple classes, helpers, or utility code, use `snake_case` (for example `trade_utils.hpp`).

## Entity naming

- Class, struct, and enum names use `CamelCase`.
- Method names use `snake_case`.

## Method naming

- Methods must be named in `snake_case`.
- Getter names may omit `get_` when they return a direct property/reference/value with no substantial computation.
- Use `get_` when the method computes a result or when omitting `get_` would make semantics unclear.

## Change scope

- Keep diffs minimal and focused.
- Do not refactor or apply style changes beyond the lines you directly touch.

## Code style

- Use 4 spaces for indentation; do not use tabs.
- Indent all declarations and definitions inside `namespace` blocks by one level (4 spaces).
- Keep opening braces on the same line for classes, methods, and namespaces.
- Do not use `using namespace`; always qualify names (for example `std::`).
- Keep project headers before system headers in include lists.
- Header files must start with `#pragma once`; if an include guard is additionally used, prefer `_MGC_PLATFORM_*_HPP_INCLUDED`.

## Header / Implementation ownership

- For generic `.hpp` / `.ipp` / `.tpp` ownership and include-structure policy, use `../docs/header-implementation-guidelines.md`.
- For the execution-focused AI-agent checklist, use `header-implementation-guidelines.md`.
- Keep this file focused on naming and formatting; do not duplicate the full ownership policy here.

## Singleton / service storage

- For generic header-only singleton / service storage and ODR-safe ownership policy, use `../docs/singleton-storage-guidelines.md`.
- For the execution-focused AI-agent checklist, use `singleton-storage-guidelines.md`.
- Keep this file focused on naming and formatting; do not duplicate the full singleton storage policy here.

## Build toolchain

- Use CMake with generator `MinGW Makefiles`.
- Use MinGW compilers (`gcc`, `g++`).
- Do not use `Ninja`.
- Do not use `NMake` / `nmake`.

## Constants and macros

- Constants and macro names use `UPPER_SNAKE_CASE`.
- Enum values use `CamelCase`.
- Mark non-inheritable classes with `final`.
- Use `override` for overridden virtual methods.
