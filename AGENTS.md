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
