# Contributing Guidelines

Thank you for considering contributing to this project!
This document explains how to set up your environment, follow our coding style, and submit contributions.

---

## 📦 Project Setup

1. **Fork and clone** the repository.
```bash
git clone git@github.com:Mirdrack/c-server.git
cd c-server
```

2. **Create a new branch** for your work:
```bash
git checkout -b feat/my-new-feature
```

3. **Build the project**:
```bash
make
```

4. **Run tests:**
```bash
make test
```

## 📝 Commit Messages
We use the [Conventional Commits](https://www.conventionalcommits.org/) specification.
This makes commits more readable and allows automatic changelog generation.

Format:
```bash
<type>(optional-scope): <short description>
```

**Common types:**

`feat:` – a new feature

`fix:` – a bug fix

`chore:` – maintenance tasks (build, dependencies, etc.)

`docs:` – documentation changes

`style:` – formatting (no code change)

`refactor:` – code restructuring without changing behavior

`test:` – adding or updating tests

`perf:` – performance improvements


## 📖 Commit Descriptions & Imperative Style
**Imperative mood:** Write commit summaries like giving a command.

✅ `fix: resolve buffer overflow in parser`

❌ `fixed: resolved buffer overflow in parser`

❌ `fix: resolves buffer overflow in parser`

**Short summary** (first line):

Keep it under **72 characters**.

State __WHAT__ the change does and __WHY__ the change is being done, not how.

**Description/body** (optional but recommended for non-trivial changes):

Leave a blank line after the summary.

Use complete sentences.

Explain __WHY__ the change was made and any important context.

Mention trade-offs or limitations if relevant.
```bash
feat(network): add IPv6 support to socket layer

This change introduces IPv6 handling in the socket API.
It ensures backward compatibility with existing IPv4 code
paths and adds new unit tests for IPv6 connections.

```


## 🧑‍💻 Coding Style
* Follow **C99** standard (or project’s configured standard).

* Use **4 spaces** for indentation, no tabs.

* Keep lines **≤ 80** characters when possible.

* Place braces on a new line:
```c
if (condition)
{
    do_something();
}
```

* Use meaningful variable and function names.

* Add comments for non-obvious logic.

* Header guards in headers:
```c
#ifndef PROJECT_HEADER_H
#define PROJECT_HEADER_H

// Declarations

#endif /* PROJECT_HEADER_H */
```


## ✅ Testing

All new features must include tests.

We use the [Check](https://libcheck.github.io/check/) framework for unit testing.

Run the test suite with:
```bash
make test
```


## 🔄 Pull Request Workflow

1. Sync your branch with `main`:
```bash
git pull origin main
```

2. Ensure code compiles without warnings:
```bash
make clean && make
```

3. Run all tests:
```
make test
```

4. Submit a PR with a clear description:

* What you changed

* Why you changed it

* Any considerations for reviewers


## 🏷️ Versioning
We follow [Semantic Versioning](https://semver.org/)
Breaking changes must be documented and reflected in the major version.
