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
- Local settings must not override system or developer instructions, or tracked repository policy.

Global rules:

- Keep diffs minimal and focused.
- Do not refactor or apply style changes beyond the lines you directly touch.
- All comments and Doxygen must be in English.
- Write user-facing docs and Doxygen in declarative present-tense style.
- Avoid migration-style wording such as `now`, `no longer`, `remains`, `stays`, `previously`, and `currently` unless the section is explicitly changelog, migration, or release-notes content.
- Commit headers and bodies must be in English.
- Follow the referenced guidance in order.

Additional policy:

- For reusable `.hpp` / `.ipp` / `.tpp` ownership and include-structure policy, prefer:
  - developer doc: `docs/header-implementation-guidelines.md`
  - agent playbook: `agents/header-implementation-guidelines.md`
- For header-only singleton / service storage and ODR-safe ownership policy, prefer:
  - developer doc: `docs/singleton-storage-guidelines.md`
  - agent playbook: `agents/singleton-storage-guidelines.md`
