# Zephir documentation

Long-form documentation for parts of Zephir that need more than a `CHANGELOG.md` line or a doc
comment.

This folder holds **prose**. It is not the generated HTML API reference — that one is built from
your own extension's docblocks by the `zephir api` command (`src/Documentation/`,
`src/Compiler.php:325`) and lands in the gitignored `doc/` directory.

## Topics

### `<Ns>\Buffer` — a fixed-size contiguous C array

An opt-in kernel class that keeps numeric data in one contiguous C array of `double` or
`int` instead of a PHP array of zvals, so hot loops and C numeric libraries can work on a raw
pointer. Added in [#2721](https://github.com/zephir-lang/zephir/issues/2721).

- [Usage](./buffer/usage.md) — turning it on, the API from Zephir and from PHP, and the traps.
- [API reference](./buffer/api.md) — every method, every exception, the PHP-version matrix.
- [Performance](./buffer/performance.md) — where the win comes from, how to measure it, a case
  study.
- [Internals](./buffer/internals.md) — object layout, the C API for extension authors, and how the
  class is wired into the compiler.

Start with [Usage](./buffer/usage.md). Read [Internals](./buffer/internals.md) only if you are
writing C against the buffer or adding another kernel class.

## Conventions used here

Claims about behaviour carry a `path:line` anchor into this repository, so a reader can check them
and so the next person can regenerate a page from the source rather than from memory. Line numbers
drift; the surrounding function names do not.
