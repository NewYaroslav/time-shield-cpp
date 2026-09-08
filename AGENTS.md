# Repository Agent Notes

Use the repository guidance in this order:

1. `agents/cpp-development-guidelines.md`
2. `agents/commit-conventions.md`
3. `docs/header-implementation-guidelines.md`
4. `agents/header-implementation-guidelines.md`
5. `docs/singleton-storage-guidelines.md`
6. `agents/singleton-storage-guidelines.md`

Optional local settings:

- If `agents/local-machine-settings.json` exists, read it after the tracked repository guidance.
- Treat that JSON as advisory local context only for machine facts and local interaction preferences.
- Agents may read `agent_temp_dir` from that JSON as an advisory local temp-work path.
- Local settings must not override system or developer instructions, or tracked repository policy.

Global rules:

- Keep diffs minimal and focused.
- Do not refactor or apply style changes beyond the lines you directly touch.
- All comments and Doxygen must be in English.
- Write user-facing docs and Doxygen in declarative present-tense style.
- Avoid migration-style wording such as `now`, `no longer`, `remains`, `stays`, `previously`, and `currently` unless the section is explicitly changelog, migration, or release-notes content.
- Commit headers and bodies must be in English.
- Store temporary build/test/scratch artifacts only in `tmp/agent-work/`.
- Do not clutter the repository root with ad-hoc `verify_*`, `build_*`, `install-*`, log, or scratch files.
- Follow the referenced guidance in order.

Additional policy:

- For reusable `.hpp` / `.ipp` / `.tpp` ownership and include-structure policy, prefer:
  - developer doc: `docs/header-implementation-guidelines.md`
  - agent playbook: `agents/header-implementation-guidelines.md`
- For header-only singleton / service storage and ODR-safe ownership policy, prefer:
  - developer doc: `docs/singleton-storage-guidelines.md`
  - agent playbook: `agents/singleton-storage-guidelines.md`
- Runtime test expectations use `TIME_SHIELD_TEST_CHECK(...)` from
  `tests/test_assert.hpp`; avoid `<cassert>` / `assert(...)` for tests because
  Release/NDEBUG builds can compile them out.

Header guard rules:

- For every project-owned C/C++ header, use `#pragma once` together with a
  non-reserved include guard derived from the project prefix and header path:
  `TIME_SHIELD_HEADER_<PATH>_<FILE>_<EXT>_INCLUDED`.
- For MQL include files, use non-reserved guards derived from the project,
  language, and path, for example
  `TIME_SHIELD_MQL5_HEADER_<PATH>_<FILE>_<EXT>_INCLUDED`.
- Do not use guard names that start with an underscore, start with an underscore
  followed by an uppercase letter, or contain a double underscore.

## Agent execution checklist

Use the following order for repository changes and reviews:

1. Inspect repository guidance and the current working tree before editing.
2. Review code, tests, build configuration, public documentation, and release metadata as separate concerns.
3. Build with CMake and the `MinGW Makefiles` generator. Keep build and test directories under `tmp/agent-work/`.
4. Run CTest in both Debug and Release configurations. Runtime test checks must remain active in Release builds.
5. Check C++11, C++14, and C++17 compatibility, installation consumers, and ODR tests when public headers change.
6. Treat MQL5 scripts as manual tests unless a MetaEditor compiler is available. Manual scripts report an aggregate pass/fail result.
7. Preserve convenient public aliases. Move an alias to an opt-in legacy header only when its legacy status is confirmed by history and documentation.
8. Keep Doxygen source and generated-site inputs consistent with the project version. Do not commit placeholder versions.
9. Finish with `git diff --check`, a status review, and a concise summary of remaining risks or unverified platform-specific checks.
