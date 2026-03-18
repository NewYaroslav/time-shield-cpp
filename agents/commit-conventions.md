# Commit Conventions

## Format

- Commit messages must be in English and use Conventional Commits.
- A commit body is required and must describe the change.
- Use:

```bash
git commit -a -m "type(scope): short summary" -m "Detailed description of changes."
```

## Allowed types

- `feat` - new functionality.
- `fix` - bug fix.
- `refactor` - refactoring without behavior changes.
- `perf` - performance improvements.
- `test` - add or modify tests.
- `docs` - documentation changes.
- `build` - build system or dependency updates.
- `ci` - CI/CD configuration changes.
- `chore` - maintenance tasks that do not affect production code behavior.
