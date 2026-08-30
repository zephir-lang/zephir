# Change Log
All notable changes to this project will be documented in this file.

The format based on [Keep a Changelog](https://keepachangelog.com)
and this project adheres to [Semantic Versioning](https://semver.org).

## [Unreleased]

### Fixed
- Fixed a `-Wformat` warning in `zephir_fclose()` by casting the resource handle to `zend_long` and formatting it with the portable `ZEND_LONG_FMT` macro instead of `%d`, preserving compatibility with PHP 8.0's `int` handle.
- A variable whose only consumer is a closure's `use (...)` clause now counts as used: it is no longer reported as `unused-variable`, and it is declared in the generated C. A declared-but-unassigned capture was skipped by both, so the generated code referenced an undeclared identifier and the extension failed to build. Capturing a variable that was never declared now fails with `Cannot capture variable 'x' because it wasn't declared` instead of a PHP fatal error [#2029](https://github.com/zephir-lang/zephir/issues/2029)
- Fixed array defaults on a class (typed `array` property defaults, trait array property defaults and array class constants), which were one shared table that every instance mutated in place instead of being copy-on-write [#2651](https://github.com/zephir-lang/zephir/issues/2651)
- Fixed the unchecked length arithmetic in the `zephir_concat_*()` helpers, which now throw `String size overflow` like PHP instead of writing past the allocation [#2657](https://github.com/zephir-lang/zephir/issues/2657)
- Fixed `let s .= n` with an integer operand calling the declared but never defined `zephir_concat_self_long()`, so the extension built and then died at load with `undefined symbol`, and made the same append work on a `var` left-hand side [#2660](https://github.com/zephir-lang/zephir/issues/2660)
- Fixed closure `use (...)` captures being shared by every closure created from the same source line, and implemented `use (&x)` as a real reference [#2652](https://github.com/zephir-lang/zephir/issues/2652)
- Fixed the closure `use (...)` clause dropping `const`, which is now a read-only capture, and a by-reference capture of an explicitly typed local [#2653](https://github.com/zephir-lang/zephir/issues/2653)
- Fixed every closure invocation leaking one reference of the closure object and of each captured value [#2652](https://github.com/zephir-lang/zephir/issues/2652)

## [1.3.0] - 2026-08-25

### Added
- Anonymous functions can now declare a return type: `function (a) -> int { }`, including after a `use (...)` clause (`function () use (base) -> string { }`), and with `void`, union (`int|string`) and `<Class>` types. The type is declared on the closure's `__invoke`, so Reflection reports it. Requires Zephir Parser 2.6.0 [#1841](https://github.com/zephir-lang/zephir/issues/1841)
- Class constant initializers and class/trait property defaults accept a full constant expression instead of a single literal, so `const INT8_MIN = -0x7f - 1;`, `const MASK = 0xff << 8 | 0x0f;` and `public size = 1024 * 8;` now compile. Arithmetic, bitwise, concatenation, comparison, logical and ternary operators are supported, as are parentheses, other constants (`self::KB * 2`) and expressions inside array constants; the result is folded to a literal at compile time with PHP's own semantics (`4 / 2` is an `int`, `PHP_INT_MAX + 1` a float). An initializer that is not resolvable at compile time is now rejected by name instead of failing as a syntax error. Parameter defaults are unchanged. The new syntax needs Zephir Parser 2.7.0; an older extension is bypassed in favour of the built-in parser (see below) [#2061](https://github.com/zephir-lang/zephir/issues/2061)
- Added an optional `--CONFIG--` section to the `.zept` format, holding a JSON object merged into the throwaway project's `config.json`. This makes a case buildable under a non-default compiler configuration (e.g. `{"optimizations": {"internal-call-transformation": true}}`); `namespace` and `name` stay derived from the sources [#2021](https://github.com/zephir-lang/zephir/issues/2021)
- Added destructuring assignment: `let [a, b, c] = expr;` assigns consecutive array elements to several variables at once, and `let [a, , c] = expr;` skips slots. The right-hand side is evaluated exactly once, a slot past the end of the source array is `null` (as in PHP's `list()`), and a non-array source is rejected at compile time. Nested (`let [[a, b], c] = expr;`) and keyed destructuring are not supported [#2496](https://github.com/zephir-lang/zephir/issues/2496)

### Changed
- The minimum `ext-zephir_parser` accepted as the fast parsing path is now 2.7.0 (was 2.1.0). An installed extension older than that cannot parse everything the compiler accepts, so it is ignored and the built-in pure-PHP parser is used instead — compiling still succeeds, just without the C parser's speed. Upgrade the extension to restore it [#2061](https://github.com/zephir-lang/zephir/issues/2061)
- `zephir build` and `zephir install` now report where the extension was installed (`Extension installed into /usr/local/lib/php/extensions/no-debug-non-zts-20230831`), read from the `EXTENSION_DIR` the build was configured with, so it names the right directory for a build retargeted with `--with-php-config`. The plain `Extension installed.` is kept when the destination cannot be confirmed. The `Add "extension=…"` hint now names the configured `extension-name` rather than the namespace [#2467](https://github.com/zephir-lang/zephir/issues/2467)

### Fixed
- Fixed a `break` written after a `return` (or `throw`) inside a `switch` clause being reported as `Unreachable code` and, worse, stopping the `switch` from satisfying the method's return-type hint, so the snippet in the issue failed with `Reached end of the method without returning a valid type specified in the return-type hints`. A dead `break`/`continue` is a no-op that PHP accepts without any diagnostic, and the completeness check now stops at the first statement that actually transfers control instead of looking only at the last one. A dead `break` nested in an `if` arm is still honoured, because that path leaves the `switch` without returning [#1704](https://github.com/zephir-lang/zephir/issues/1704)
- Fixed the check deciding whether a method can reach its end without returning, which had three overlapping implementations and inspected only one statement of each block. It reported `Reached end of the method without returning a valid type specified in the return-type hints` for dead code written after a `return`, and conversely accepted a method that could fall off its end - an `if`/`elseif`/`else` whose `elseif` arm does not return (the `elseif` arms were never looked at), a `try` whose `catch` body is empty, or a `for`/`while` body that returns although the loop can run zero times - in which case `ZEPHIR_MM_RESTORE()` was skipped too. A single check now models `if`/`elseif`/`else`, `switch`, `try`/`catch` and loops, treating a `loop`/`while true` without a `break` as unable to fall through [#1704](https://github.com/zephir-lang/zephir/issues/1704)
- Fixed `switch` clauses not falling through as they do in PHP. A clause that did not `break` ran the `default` body instead of the next clause's body, because every clause was emitted as an independent `if` with `default` appended unconditionally; `switch a { case 1: let r .= "a"; case 2: let r .= "b"; default: let r .= "d"; }` gave `"ad"` for `1` instead of `"abd"`. Clause bodies are now emitted in source order behind a `goto` dispatch chain, so fall-through, a `default` written in the middle, and `continue` behaving as `break` inside a `switch` all match PHP. A `case` expression after the first match is no longer evaluated either [#1704](https://github.com/zephir-lang/zephir/issues/1704)
- Fixed a `use` statement naming a class of an optional extension warning as `does not exist at compile time` when that extension is not loaded in the PHP running Zephir. The bundled prototypes declare those classes, but they were required after the `use` validation, so building Phalcon without `ext-redis` or `ext-memcached` reported `Redis`, `RedisCluster` and `Memcached` as nonexistent. The prototypes — and any `prototype-dir` of the project — are now loaded before the validation. `RedisException`, which no prototype declared, was added to the `redis` prototype [phalcon/cphalcon#17517](https://github.com/phalcon/cphalcon/issues/17517)
- Fixed a method declared by a parent interface not being resolved, so `abstract class Base implements Outer` with `interface Outer extends Inner` rejected `this->go()` with `Class 'Base' does not implement method: 'go'` unless the class implemented `Inner` directly. An interface now reports the methods of its parent interfaces at any depth, including when it extends several. A concrete class is likewise held to the whole chain at build time instead of failing later as a PHP fatal error [#2635](https://github.com/zephir-lang/zephir/issues/2635)
- Casts now accept every source type. The cast operator enumerated sources per target, so more than half of all combinations failed with `Cannot cast: X to Y` — among them `(string) 5`, `(string) "abc"`, `(array) 5`, `(bool) []`, `(char) 65`, `(int)` of a `long`, and every use of `(uint)`, `(ulong)`, `(uchar)` and `(var)`. Results match PHP, including `(bool) 0.4` being `true` where a C cast would truncate it to `false` [#1841](https://github.com/zephir-lang/zephir/issues/1841)
- Fixed `(array)` and `(object)` casts overwriting the variable being cast: both lower to kernel conversions that run in place, so `let b = (array) a` turned `a` itself into an array [#1841](https://github.com/zephir-lang/zephir/issues/1841)
- Fixed an interface `extends`ing a bundled interface declared in a PHP extension header (e.g. `interface I extends \JsonSerializable, \SeekableIterator`) failing to compile with `'php_json_serializable_ce' undeclared`. The required `#include`s were collected but then discarded for interface files, so the emitted `zend_class_implements()` calls referenced undeclared class entries [#2427](https://github.com/zephir-lang/zephir/issues/2427)
- Fixed the `internal-call-transformation` optimization breaking method overriding. It replaced `this->method()` with a direct call to the callee's C function, which skips PHP's dispatch, so a subclass override was never reached — `Vector::multiply()` calling `this->multiplyMatrix()` always ran `Vector`'s version even on a `ColumnVector`. The direct call is now used only where PHP resolves the target statically anyway: a `final` method, a `private` method of the class being compiled, or `this` in a `final` class. `self::`, `parent::` and `Class::method()` calls are unaffected, and a userland PHP class extending a Zephir one now overrides correctly too. Accessors generated from `{get}`/`{set}` are always public, so they no longer take the direct call [#2021](https://github.com/zephir-lang/zephir/issues/2021)
- Fixed `internal` methods with parameters reading the caller's arguments instead of their own, which crashed the process outright whenever the two arities differed. They are called C-to-C with the caller's `execute_data` and receive their real arguments as trailing pointers, but still ran a `ZEND_PARSE_PARAMETERS` block against that frame; the block is now skipped and every parameter type is bound from the arguments actually passed. Affects the `internal` keyword as well as `internal-call-transformation` [#2021](https://github.com/zephir-lang/zephir/issues/2021)
- Fixed a segfault whenever an exception is created inside an `internal` method — which includes every `throw` out of one, since the constructor runs first. Such a method is called C-to-C on a synthetic `zend_execute_data` whose `func` was `NULL`, and the engine reads a `func`-less frame as a generator placeholder frame only, so the backtrace capture that every Exception constructor performs dereferenced it. The frame now carries an anonymous internal function and is skipped in userland backtraces. `debug_backtrace()` inside an `internal` method crashed for the same reason. Affects the `internal` keyword and `internal-call-transformation` on PHP 8.1 and later [#2639](https://github.com/zephir-lang/zephir/issues/2639)
- Fixed a `.zept` case being written to the wrong `.zep` path when a `--FILE--` comment happens to contain the words `class`, `interface` or `trait` followed by a name. The path is derived by matching the declaration over the whole section body, and prose such as "a method of the class being compiled" won, so the build failed with `Unexpected class name ... in file: 'stub/being.zep'`. A declaration now has to begin a statement [#1098](https://github.com/zephir-lang/zephir/issues/1098)
- Fixed `internal-call-transformation` emitting body-less C functions for `abstract` methods and for generators, and dropping `static` from the generated twin. It also generated each parameter conversion twice, because the twin shared the original method's parameter list — a leak for `string` and `array` parameters, which allocate [#2021](https://github.com/zephir-lang/zephir/issues/2021)
- Fixed capturing a `string` in a closure emitting C that does not compile (`ZVAL_STRING` applied to a `zval`). It affected every `string` held as a zval: a local, a parameter reassigned in the method body, and — with `internal-call-transformation` enabled — any `string` parameter, which was the last file of `stub/` failing to build under that option. Such a capture is no longer boxed a second time [#2638](https://github.com/zephir-lang/zephir/issues/2638)
- Fixed `__FUNCTION__` and `__METHOD__` reporting a compiler-generated method name. Both are folded at compile time from the method being compiled, and the compiler recompiles a method body under a mangled name in two cases: the twin `internal-call-transformation` generates (`<name>_zephir_internal_call`) and the step a generator's body is moved into (`zephir_gen_step_<name>`). They now report the name declared in the `.zep` source, as do compile diagnostics that name the method [#2643](https://github.com/zephir-lang/zephir/issues/2643)
- Fixed the stale shared object not being removed before a full rebuild in projects that set `extension-name`: the cleanup looked for `ext/modules/<namespace>.so`, while the module is built as `ext/modules/<extension-name>.so` [#2467](https://github.com/zephir-lang/zephir/issues/2467)
- Fixed `zephir install` and `zephir build` reporting success when the extension was never installed. The exit status of `sudo make install` was ignored — only the presence of `ext/modules/<name>.so`, which plain `make` produces, was checked — and the CLI discarded the result of the install step on top of that. A missing sudo, a declined password or a read-only extension directory now fails with the exit code and a non-zero status instead of printing `Extension installed.` [#2467](https://github.com/zephir-lang/zephir/issues/2467)

## [1.2.0] - 2026-07-27

### Added
- Added the `::class` magic constant: `self::class`, `parent::class` and `ClassName::class` resolve to the fully-qualified class name at compile time, and `static::class` resolves to the called class at runtime (late static binding) [#2527](https://github.com/zephir-lang/zephir/issues/2527)
- Trait properties can now have array default values (e.g. `trait T { public opts = [1, 2, 3]; }`, including empty, keyed and nested arrays). The default is materialized as a persistent immutable array on the trait's class entry, so it is carried into both Zephir classes and PHP userland classes that `use` the trait, matching native PHP trait semantics. Static array defaults on traits remain rejected (Zephir resolves `self::` in a trait to the trait itself, so each using class could not get its own copy) [#2607](https://github.com/zephir-lang/zephir/issues/2607)
- Class and trait properties can now declare a type, mirroring PHP's typed properties: builtin (`int`, `bool`, `double`, `string`, `array`, `object`), nullable (`?string`), and class types (`<My\Class>`, `?<My\Class>`). The type is emitted via `zend_declare_typed_property`, so Reflection reports it and the engine enforces it; a typed property with no default is uninitialized (`public <Foo> conn;`) rather than `null`, matching PHP [#2608](https://github.com/zephir-lang/zephir/issues/2608)
- Added union types (`int | float`) on method/function parameters and class/trait properties (e.g. `public int|float foo;`, `function f(float|int bar) -> int|float`). Reflection reports a `ReflectionUnionType`; union properties are engine-enforced on write (scalar, `null`/`false`, and one or more `<Class>` members), while a union parameter binds as a mixed value carrying its declared arg-info type mask. Union return types were already supported [#2613](https://github.com/zephir-lang/zephir/issues/2613)
- Added `readonly` properties (e.g. `public readonly array foo`): the property carries `ZEND_ACC_READONLY`, so Reflection reports it and the engine enforces write-once after initialization. Like PHP, a readonly property must be typed, cannot declare a default value and cannot be static (each rejected at compile time). Enforced on PHP 8.1+; on PHP 8.0 it degrades to a plain typed property [#2614](https://github.com/zephir-lang/zephir/issues/2614)
- Added the `+=` (add-assign) operator on arrays, performing PHP array union (`let a += [1, 2]`), for array literals and both `array`-typed and untyped `var` operands [#1280](https://github.com/zephir-lang/zephir/issues/1280)
- Added an opt-in `missing-optimizer` warning that reports function calls with no applicable optimizer, which are dispatched by name through the Zend function table at runtime instead of being inlined. It also covers calls an existing optimizer declined because it does not handle that call form (e.g. `count(a, 0)`), which are otherwise invisible. Enable with `-wmissing-optimizer`; each function is reported once per run, at its first call site [#2468](https://github.com/zephir-lang/zephir/issues/2468)
- Added a `--with-php-config=PATH` option to the `build`, `compile` and `fullclean` commands, so an extension can be built against a PHP installation other than the one in the `PATH` (e.g. a separate ZTS build). `phpize` is taken from the same directory as the given `php-config`, and an unusable or mismatched path is rejected before the build starts. Not available on Windows, where the dev-pack (`%PHP_DEVPACK%`) selects the target PHP [#1834](https://github.com/zephir-lang/zephir/issues/1834)

### Fixed
- Fixed `char`/`uchar` values in dynamic (zval) contexts. A string offset read (`s[i]`) now boxes as a 1-character string when its target is a `var`/`string` variable or an array element, matching PHP's `$s[$i]`: `string ch; let ch = s[i];` used to emit C that did not compile, and `let pieces[] = s[i];` was rejected with `Unknown type: uchar`. A declared `char`/`uchar` variable still boxes as its integer byte value (as `let a = [ch]` and `return ch` already did) — use `(string) ch` for the character — and a direct `return s[i]` is unchanged [#1629](https://github.com/zephir-lang/zephir/issues/1629)
- `char`/`uchar` are now accepted in the remaining scalar positions, each of which previously threw or emitted invalid C: `(uchar)` casts, `(string)`/`(char)` casts of a `uchar`, `uchar` method parameters and globals, keyed array literals (`[1: ch]`), `let a[k] = ch`, object and static property array offsets, and `double`/`bool` targets. A `char` literal reaching a numeric slot no longer emits `ZVAL_LONG(&x, ''a'')`, and assigning an `int` to a `string` (`string s; let s = i;`) no longer emits dead PHP-5 code [#1629](https://github.com/zephir-lang/zephir/issues/1629)
- Fixed `acos()` and `asin()` never using their optimizers on case-sensitive filesystems: optimizer lookup derives `AcosOptimizer.php`/`AsinOptimizer.php` from the function name, but the files were named `ACosOptimizer.php`/`ASinOptimizer.php`, so both calls compiled to a runtime function call on Linux while macOS and Windows inlined them [#2468](https://github.com/zephir-lang/zephir/issues/2468)
- Fixed typed-property and array-class-constant string defaults containing escape sequences (e.g. `protected string x = "A\\B\\C"`) being emitted with the wrong byte length: the length was `strlen()` of the source-escaped PHP value (a `\\` counted as two bytes) while the C string literal is shorter, so the runtime string was over-long and corrupted with trailing garbage bytes. The length is now measured by the C compiler (`SL()` / `sizeof`), correct for every escape sequence [#2617](https://github.com/zephir-lang/zephir/issues/2617)
- Fixed the `Zephir version has changed` warning firing on every rebuild: the version-cache directory existence check used `is_file()` on a path that is always a directory, so the check never matched and the warning degraded to "warn whenever the project was already built". It now fires only on a genuine version change and names the old and new versions [#2621](https://github.com/zephir-lang/zephir/issues/2621)

## [1.1.0] - 2026-07-14

### Added
- Implemented generator (`yield`) support: methods containing `yield;` / `yield expr;` / `yield key, value;` now compile to lazy generators returning an extension-scoped `<Namespace>\Generator` object (`\Iterator` + `send()` + `getReturn()`) with PHP-identical laziness, auto-key, rewind and exhaustion semantics. Suspension works across `while`/`loop`/`if`/`switch`, `for in <array>` (snapshot iteration), `range()` and object iteration; `yield` inside `try/catch`, `for ... in iterator(...)`, closures and functions is rejected at compile time. PHP's `\Generator` is final, so declare `-> <\Iterator>`, `-> <\Traversable>` or omit the return type [#1849](https://github.com/zephir-lang/zephir/issues/1849)
- Added trait support: `trait Name { }` and import inside classes/traits. Zephir traits compile to real `ZEND_ACC_TRAIT` class entries, so PHP userland can `use` them [#504](https://github.com/zephir-lang/zephir/issues/504)

### Changed
- Multi-dimensional array assignments (`let a[i][j] = v`) now skip a redundant final-offset hash lookup in `zephir_array_update_multi_ex` (three hash operations per write down to two), making tight-loop matrix builds ~15–20% faster (measured via `tests/Benchmark/MultiDimArrayBench.php`) [#1884](https://github.com/zephir-lang/zephir/issues/1884)
- Object property reads and writes on `this` with a compile-time-known name now reuse a per-method interned `zend_string` for the name and a per-request inline property cache slot (`zephir_read_property_cached` / `zephir_update_property_zval_cached`), instead of allocating a `zend_string` and re-resolving the property offset by hash lookup on every access. This takes `this->prop` reads from ~0.44× to ~1.35× of native PHP throughput — now faster than interpreted PHP (measured via `tests/Benchmark/PropertyAccessBench.php`) [#1902](https://github.com/zephir-lang/zephir/issues/1902)

### Fixed
- Fixed arithmetic compound-assignment (`+=`, `-=`, `*=`) on an object property overwriting the property instead of accumulating when the right-hand side is a variable (e.g. `let this->total += delta;` compiled to `let this->total = delta;`, dropping the read). The current property value is now read, the operator applied, then written back — matching the already-correct literal-operand path. Arithmetic sibling of the concat-assign fix [#2063](https://github.com/zephir-lang/zephir/issues/2063); found while adding the property inline-cache [#1902](https://github.com/zephir-lang/zephir/issues/1902)
- Pure-PHP parser parity: a closure-arrow (`x => expr`) now requires a bare identifier on the left, matching the C parser grammar `yield "k" => 2;` and `let f = "x" => 2;` are syntax errors again instead of being mis-parsed [#1849](https://github.com/zephir-lang/zephir/issues/1849)
- `for` loops over the `..`/`...` range operators (`for i in 0..n`) now compile to an integer counting loop instead of materializing an intermediate array, matching the existing `for i in range(...)` optimization; this also lets a typed `int` loop variable be used with `..`, which previously failed to compile [#2433](https://github.com/zephir-lang/zephir/issues/2433)
- A static property initialized with an array literal no longer breaks single-file (concatenated) builds: the `zephir_init_static_properties_<Class>()` initializer is now forward-declared at file scope in the project `.c` instead of the per-class header, keeping its declaration and definition in one translation unit so their linkage cannot mismatch (`static` vs non-`static`) [#2601](https://github.com/zephir-lang/zephir/issues/2601)

## [1.0.0] - 2026-07-01

### Added
- Implemented `.zept` (Zephir Test) format and a `zephir test <path>` command: a single file bundles the Zephir implementation (`--FILE--`, repeatable), the PHP usage (`--USAGE--`) and the expected output (`--EXPECT--`/`--EXPECTF--`). Each case is compiled into a throwaway extension (pure-PHP parser forced) and its output checked, so a `.zept` doubles as an end-to-end parser test. A dependency-free runner (`Zephir\Zept\`) backs both the CLI and the `tests/Zephir/BlackBox/ZeptSuiteTest.php` bridge [#1098](https://github.com/zephir-lang/zephir/issues/1098)
- Built-in pure-PHP Zephir parser, removing the hard dependency on the `ext-zephir_parser` PECL C extension. The parser produces the **identical** intermediate representation (IR) the compiler already consumes — verified byte-for-byte (`json_encode` equality) against the C extension over the full `stub/` corpus, the extracted `php-zephir-parser` test snippets, and the entire Phalcon framework (930/930 `.zep` files). Building the bundled stub extension with the PHP parser generates a byte-identical C tree and passes the Extension test suite identically to the C-extension build [#2495](https://github.com/zephir-lang/zephir/issues/2495)

### Changed
- The `ext-zephir_parser` C extension is now **optional**: it is used as a fast path when loaded, otherwise the built-in PHP parser handles parsing. Set the `ZEPHIR_FORCE_PHP_PARSER` environment variable to force the PHP backend even when the extension is present (useful for differential testing and deterministic builds). `Zephir\Parser\Manager::isAvailable()` is now always `true` [#2495](https://github.com/zephir-lang/zephir/issues/2495)

### Deprecated
- The `!` (strict type) modifier on argument types (e.g. `int! a`) now emits a deprecation notice during `zephir generate`. A future parser will no longer recognize it. Suppress with `-Wdeprecated-strict-type` [#2274](https://github.com/zephir-lang/zephir/issues/2274)
- `RedisCluster` prototype (in `prototypes/redis.php`), so Zephir code can resolve its methods and constants when ext-redis is not installed

## [0.23.0] - 2026-06-06

### Added
- Support for array class constants (e.g. `const BAR = [1, 2, 3];`, including nested and keyed arrays). The value is materialized on the class entry as a persistent immutable array (readable from PHP), and `self::CONST` resolves to it in Zephir code [#2533](https://github.com/zephir-lang/zephir/issues/2533)
- Full union return type support, enforced by the engine just like a hand-written PHP union return type. Any combination of classes and/or scalar types is now emitted into the compiled extension's arginfo (via `ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX` / `ZEND_BEGIN_ARG_WITH_RETURN_OBJ_TYPE_MASK_EX`), so `-> <Model> | <Row> | null`, `-> int | string`, `-> <Foo> | int`, etc. are reported by Reflection and enforced at runtime. Previously only `int | false` / `T | null` carried a return type and every other union silently emitted none [#2428](https://github.com/zephir-lang/zephir/issues/2428)

### Changed
- Cached method/static calls now reuse the resolved function handler instead of repeating the method-table lookup in `populate_fcic()`, shaving ~6–10% off per-call dispatch in hot loops (measured via `tests/Benchmark/CallDispatchBench.php`) [#1510](https://github.com/zephir-lang/zephir/issues/1510)
- **BC**: methods declared with a multi-type union return now carry an enforced return type in the compiled extension. PHP subclasses that override such a method must declare a compatible (equal or narrower) return type, where previously the missing return type made any override valid [#2428](https://github.com/zephir-lang/zephir/issues/2428)

### Fixed
- Fixed `let self::prop = "";` (assigning an empty string to a static property) failing to compile. The generated `ZVAL_EMPTY_STRING` was passed the bare variable name instead of its address [#2409](https://github.com/zephir-lang/zephir/issues/2409)
- Fixed double-quoted string escapes not matching PHP: `\u{XXXX}` Unicode codepoints are now UTF-8 encoded (previously emitted literally), and `\xHH` no longer absorbs following hex digits (e.g. `"\x41BC"` is now `"ABC"`). Out-of-range codepoints (`> U+10FFFF`) and a `\x` with no hex digit are emitted safely instead of producing malformed C [#2030](https://github.com/zephir-lang/zephir/issues/2030)
- Fixed assigning a non-constant boolean expression to a dynamic property (e.g. `let this->{key} = value > 5;`) failing to compile with a cryptic `?` error; it now branches at runtime like a regular property. The remaining `?` placeholders in `Backend::updateArray()` and PHP-constant resolution were replaced with descriptive messages [#1790](https://github.com/zephir-lang/zephir/issues/1790)
- Fixed hexadecimal literals (e.g. `0xffffffff << 32`, `0xff + 1`) raising "A non-numeric value encountered" during constant folding. Numeric literals are now decoded to real ints before folding in the bitwise and arithmetical operators [#2014](https://github.com/zephir-lang/zephir/issues/2014)
- Fixed array writes to a property holding an `ArrayAccess` object converting it to a plain array. `let this->prop[key] = value;` now calls the object's `offsetSet()`, and a chained `let this->prop[a][b] = value;` now mirrors native PHP exactly (fetches `offsetGet()` once and raises the "Indirect modification of overloaded element" notice) instead of replacing the object [#2465](https://github.com/zephir-lang/zephir/issues/2465)
- Fixed integer arithmetic with a dynamic operand losing int64 precision (e.g. `1000000000000000000 + var` rounded through a `double`). The result of `zephir_get_numberval()` is now cast to `(zend_long)` in integer contexts [#2010](https://github.com/zephir-lang/zephir/issues/2010)
- Fixed `[ERROR] Unknown char` when a `char`/`uchar` variable is used as an array literal item (e.g. `return [ch1, ch2];`). Such items are now boxed as their byte (integer) value [#1988](https://github.com/zephir-lang/zephir/issues/1988)
- Fixed a false `Unreachable code` warning on a guard reading a variable that holds a constant before a loop but is reassigned inside the loop body (e.g. `let x = false; for ... { if x { ... } let x = true; }`). The constant-folding unreachability check is now skipped inside loops [#1170](https://github.com/zephir-lang/zephir/issues/1170)
- Subtraction with a `-` glued to a digit (e.g. `range(0, len-1)`, `arr[0]-1`) no longer raises `Syntax error`; the parser now treats `-` after a value as a binary operator instead of the sign of a negative literal [#2011](https://github.com/zephir-lang/zephir/issues/2011)
- Fixed `Undefined array key "variable"` when assigning to a nested object property (e.g. `let this->bar->data = value;`). The inner object is now resolved into a temporary and the assignment is handled as a regular object-property write [#2532](https://github.com/zephir-lang/zephir/issues/2532)
- Fixed the static type-inference pass leaking debug output (`StaticTypeInference=...` / `STI=...`) to stdout during compilation when it met a variable type or expression node it did not enumerate (e.g. reassigning a var holding a `(object)` cast). Such cases now silently degrade the variable to dynamic [#1877](https://github.com/zephir-lang/zephir/issues/1877)
- Fixed concat-assign (`let this->prop .= value`) on object properties overwriting the property instead of appending to it; the current value is now read and concatenated [#2063](https://github.com/zephir-lang/zephir/issues/2063)
- Fixed a false `Reached end of the method without returning a valid type` error for a method ending in an exhaustive `switch` (one with a `default` clause that returns/throws on every path) [#1706](https://github.com/zephir-lang/zephir/issues/1706)
- Fixed `char`/`uchar` return values sign-extending when the byte exceeds 127 (e.g. `0x8A` returned `-118` instead of `138`). Char returns are now cast to `unsigned char` to preserve byte semantics [#1291](https://github.com/zephir-lang/zephir/issues/1291)
- Fixed external dependency classes (`external-dependencies` in `config.json`) not being found on case-sensitive filesystems. The `.zep` path was lower-cased wholesale, so a PSR-4 class such as `Phalcon\Support\Collection` looked for `support/collection.zep` instead of `Support/Collection.zep`. The path is now resolved using the namespace casing as written, falling back to the lower-cased path for backward compatibility [#2499](https://github.com/zephir-lang/zephir/pull/2499)
- Fixed `[ERROR] Unknown type: ...` when declaring a local with an expression default value (e.g. `var x = i + 1;`, concat, ternary, method calls). Declaration defaults now infer their type through the same path as `let` assignments [#2394](https://github.com/zephir-lang/zephir/issues/2394)
- Fixed generated PHP stubs dropping all but the first class in a union return type (e.g. `-> <Model> | <Row> | null` produced `Model|null` instead of `Model|Row|null`) [#2428](https://github.com/zephir-lang/zephir/issues/2428)
- Fixed capturing a scalar `string`-typed variable in a closure via `use()` emitting `ZVAL_STRING` on a `zend_string *` and failing to compile; it now boxes with `ZVAL_STR` and the captured value is treated as a zval inside the closure body [#2562](https://github.com/zephir-lang/zephir/issues/2562)
- Fixed default values not being readable through Reflection for parameters that have a class type, such as `<Foo> bar = null`. Before this `getDefaultValue()` threw an error and `isDefaultValueAvailable()` returned `false`. These parameters now use `ZEND_ARG_OBJ_TYPE_MASK`, which keeps the default value, instead of `ZEND_ARG_OBJ_INFO` [#2564](https://github.com/zephir-lang/zephir/issues/2564)
- Fixed class-constant parameter defaults (e.g. `string cipher = self::DEFAULT_CIPHER`) not being readable through Reflection. The `static-constant-access` default was not folded, so the arg_info stored `"null"` and `getDefaultValue()` returned `null` instead of the constant's value. The constant is now folded to its scalar literal for the arg_info, mirroring how the property declaration and method body already resolve it. Follow-up to [#2566](https://github.com/zephir-lang/zephir/pull/2566) [#2564](https://github.com/zephir-lang/zephir/issues/2564)
- Fixed `instanceof` not narrowing the type inside the guarded block. Calling a method that exists only on the subtype, as in `if (x instanceof Sub) { x->subMethod(); }`, no longer fails generation with a "does not implement method" error. The narrowed type applies only inside the `if` block [#2565](https://github.com/zephir-lang/zephir/issues/2565)
- `zephir generate` now fails with a clear error when a concrete class does not implement an abstract method inherited from an abstract parent, instead of letting the engine raise "Cannot instantiate abstract class" only at runtime [#1628](https://github.com/zephir-lang/zephir/issues/1628)
- Fixed `Unknown type ...` error when a property combines a `{get}`/`{set}` shortcut with a `@var` docblock that includes the variable name (e.g. `@var int $foo`). The generated accessor now uses only the type, dropping the variable name and any description [#2543](https://github.com/zephir-lang/zephir/issues/2543)
- Fixed a fatal error when a class constant is initialized from a predefined PHP constant (e.g. `const BAR = PHP_VERSION_ID;`). Such constants are now folded to their literal value at compile time instead of crashing on the unavailable runtime resolution path [#2542](https://github.com/zephir-lang/zephir/issues/2542)

## [0.22.0] - 2026-05-29

### Added
- Added support for variadic functions/methods using the `...` operator (e.g. `function f(int first, ...rest)`) [#2025](https://github.com/zephir-lang/zephir/issues/2025)
- Compiler now recognizes the parser-emitted `yield` AST node (bare `yield;`, `yield expr;`, `yield key, value;`) [#1849](https://github.com/zephir-lang/zephir/issues/1849)
- Added PHPBench-based runtime benchmarks suites under `tests/Benchmark/` [#2541](https://github.com/zephir-lang/zephir/issues/2541)

### Changed
- `for k, v in expr` now skips the unreachable branch when the iterand's dynamic type is known [#1878](https://github.com/zephir-lang/zephir/issues/1878)

### Fixed
- Fixed dynamic init `new {className}()` ignoring constructor visibility, allowing classes with a `protected`/`private` constructor to be instantiated from any scope [#882](https://github.com/zephir-lang/zephir/issues/882)
- Fixed `unset(obj->{variable})` (dynamic property name) and `unset(obj->{"literal"})` (string-literal brace syntax) throwing `CompilerException: Cannot use expression type: property-dynamic-access in "unset"`. `UnsetStatement` now handles `property-dynamic-access` and `property-string-access` nodes, emitting the new `zephir_unset_property_zval()` kernel helper for variable keys [#808](https://github.com/zephir-lang/zephir/issues/808)
- Fixed `continue` inside `for ... in` loops over PHP `Iterator`/`Traversable` objects producing an infinite loop [#2546](https://github.com/zephir-lang/zephir/issues/2546)
- Fixed `elseif` conditions that contain sub-expressions with side effects (such as array element access `myvar[0]`) being evaluated unconditionally before the outer `if`, causing spurious "Cannot use a scalar value as an array" notices when a preceding branch had already returned [#1097](https://github.com/zephir-lang/zephir/issues/1097)
- Fixed cross-class chained `<static>` (and `<self>`/`<parent>`) return-type resolution. The same-class case landed in #2537 by substituting the lexical class for the reserved keyword. The cross-class case — `other->returnsStatic()->method()` where `other` is a local variable typed as a different class — was still resolved against the call site's lexical class, so the chained method lookup ran on the wrong definition and the build aborted with `Class '<EnclosingClass>' does not implement method: '<method>'`. `MethodCall.php` already resolves the receiver's `$classDefinition` earlier in the function (from `$variableVariable->getClassTypes()`); the substitution now uses that definition, which is identical to `$compilationContext->classDefinition` for the `this` branch and preserves existing behavior there. [#2505](https://github.com/zephir-lang/zephir/issues/2505)

### Documentation
- Documented the workaround for `[ClassName, "protectedOrPrivateMethod"]` arrays passed as callbacks to PHP higher-order functions (`array_reduce`, `usort`, `preg_replace_callback`, etc.) [#2167](https://github.com/zephir-lang/zephir/issues/2167)

## [0.21.0] - 2026-05-20

### Added
- Add two templates to structure bug reports and feature requests. [#2525](https://github.com/zephir-lang/zephir/pull/2525)
- Added `<self>` method return-type annotation being emitted as a namespaced literal class name in arginfo [#2505](https://github.com/zephir-lang/zephir/issues/2505)

### Fixed
- Fixed PHP-stub generator emitting *implicit-nullable* parameters (`Foo $x = null`) that PHP 8.4 deprecates [#2426](https://github.com/zephir-lang/zephir/issues/2426)
- Fixed false-positive `possible-wrong-parameter` warning when a `string`, `bool`, `array`, or `callable` argument was passed to a method parameter typed `mixed` [#2512](https://github.com/zephir-lang/zephir/issues/2512)
- Fixed `use` statements registering aliases for non-existent classes silently [#2435](https://github.com/zephir-lang/zephir/issues/2435)
- Fixed `Unknown type: closure` (and other AST-type-name) errors when a `var` declaration was given [#2522](https://github.com/zephir-lang/zephir/issues/2522)
- Fixed segfault when `for k, v in iterator(x)` was given a non-Iterator operand (e.g. `stdClass`, plain array, scalar). [#820](https://github.com/zephir-lang/zephir/issues/820)
- Fixed `--vernum` CLI option silently falling through to the help screen instead of printing the numeric version. [#2454](https://github.com/zephir-lang/zephir/issues/2454)
- Fixed silent generation of uncompilable C when a local `var` was re-declared with the same name as a method parameter. [#2009](https://github.com/zephir-lang/zephir/issues/2009)
- Fixed `string` locals that were initialized to the empty string (`string s = "";` or `let s = "";`) ending up as `null` at runtime when subsequent code paths did not reassign them. [#2393](https://github.com/zephir-lang/zephir/issues/2393)
- Fixed `new static()` falling back to early binding. [#2324](https://github.com/zephir-lang/zephir/issues/2324)
- Fixed `ParseException::__construct()` triggering a fatal PHP 8.4 deprecation (`Implicitly marking parameter $previous as nullable`) when a `.zep` file had a syntax error — the deprecation suppressed the actual parse-error message. Parameter is now declared as `Exception | Throwable | null`.
- Fixed `isset()` returning `true` for object properties and array offsets whose value is `null`. Previously the `isset()` operator emitted `zephir_isset_property*` / `zephir_array_isset*` kernel helpers that only performed key/property existence checks — equivalent to `property_exists()` / `array_key_exists()` — so `isset(obj->p)` where `obj->p === null`, `isset(arr["k"])` where `arr["k"] === null`, and `isset(obj->declaredButNeverAssigned)` all returned `true` instead of PHP's `false`. The codegen now emits new `zephir_isset_property_value*` / `zephir_array_isset_value*` helpers that also reject `IS_NULL`; for objects this delegates to the `has_property` handler with `ZEND_PROPERTY_ISSET` (same path the engine uses for the `ZEND_ISSET_ISEMPTY_PROP_OBJ` opcode), so `__isset` magic and inaccessible/private property scoping behave correctly. The existing key-only helpers stay available for internal callers that genuinely want existence semantics (e.g. `array_key_exists`). [#2385](https://github.com/zephir-lang/zephir/issues/2385)
- Forcing Exception on Failure of Compile Step

## [0.20.1] - 2026-05-15

### Changed
- Migrated Black-box Testing from Sharness to PHPUnit. [#2492](https://github.com/zephir-lang/zephir/issues/2492)

### Fixed
- Fixed method return-type enforcement at runtime for `return this->property`: methods declared with a strict scalar return type (`-> string`, `-> int`, `-> double`, `-> array`) now throw `TypeError` when the property holds a mismatching value instead of silently returning it. PHP only verifies internal-function return types in `ZEND_DEBUG=1` builds, so the generated C code now emits a runtime check via new `RETURN_MEMBER_TYPED` / `RETURN_MM_MEMBER_TYPED` kernel macros. The error message matches PHP's userland format: `Class::method(): Return value must be of type X, Y returned`. Nullable return types (`string | null`) and union/`mixed` returns are left unchecked, as before. [#1991](https://github.com/zephir-lang/zephir/issues/1991), [#2196](https://github.com/zephir-lang/zephir/pull/2196)
- Fixed built-in array methods called on a `var`-typed variable (e.g. `let b = b->join("");` where `b` is declared `var`). Previously these were emitted as `ZEPHIR_CALL_METHOD(b, "join", …)` and surfaced as a `RuntimeException: Trying to call method join on a non-object` (originally a segfault, reduced to an exception over time). When the method name is one of the array-specific built-in names that can't meaningfully be a real object method (`join`, `reversed`, `rev`, `tojson`, `haskey`, `mergerecursive`, `replacerecursive`, `sortbykey`, `reversesort`, `reversesortbykey`), the compiler now dispatches via `ArrayType` — lowering to the matching PHP function call (e.g. `join(glue, array)`), just like statically-typed array variables already did. Common Iterator/Countable-style method names (`count`, `push`, `pop`, `shift`, `sort`, `next`, `current`, `end`, `key`, `reset`, `each`) are deliberately excluded so object dispatch on those is preserved. [#733](https://github.com/zephir-lang/zephir/issues/733), [#2228](https://github.com/zephir-lang/zephir/pull/2228)
- Fixed `let this->prop = value` inside closures: the closure-binding detector from #2497 caught property reads but missed let-statement writes (different AST shape), so writes were rejected as "Property not defined on stub\NN__closure". [#1873](https://github.com/zephir-lang/zephir/issues/1873), [#2203](https://github.com/zephir-lang/zephir/pull/2203)
- Reorganised `populate_fcic()` to pin calling/called scope earlier for direct call paths (`zephir_fcall_ce` and closures). **#2321 is NOT fixed by this change.** The original symptom — `preg_replace_callback(..., [this, 'privateMethod'], ...)` failing with "cannot access private method" — remains; PHP's callable-argument validation walks `prev_execute_data` for the nearest `ZEND_USER_CODE` frame and Zephir methods are `ZEND_INTERNAL_FUNCTION` so they're skipped. **Workaround**: wrap the callback in a Zephir closure that captures `this` — `function (m) { return this->privateMethod(m); }` — closures aren't re-validated by PHP. [#2321](https://github.com/zephir-lang/zephir/issues/2321), [#2325](https://github.com/zephir-lang/zephir/pull/2325)

## [0.20.0] - 2026-05-12
### Added
- Added support of PHP `8.5` [#2459](https://github.com/zephir-lang/zephir/issues/2459), [#2461](https://github.com/zephir-lang/zephir/pull/2461)
- Added closure `this` and `use` support: closures can now reference `this` to call methods and access properties of the enclosing class [#2497](https://github.com/zephir-lang/zephir/issues/2497), [#2503](https://github.com/zephir-lang/zephir/pull/2503)
- Added support for `char` and `uchar` literal/variable values as method arguments to parameters typed `char`/`uchar` [#2469](https://github.com/zephir-lang/zephir/issues/2469), [#2507](https://github.com/zephir-lang/zephir/pull/2507)
- Added support for typed constants (PHP 8.3+) in generated stubs [#2498](https://github.com/zephir-lang/zephir/pull/2498)
- Added PhpStan/Psalm tag pass-through whitelist for class-level docblocks in generated stubs [#2501](https://github.com/zephir-lang/zephir/pull/2501)
- Added validation of `@phpstan-return`, `@phpstan-var`, `@phpstan-type`, `@phpstan-param`, `@phpstan-import-type` and Psalm equivalents (delegates parsing to `phpstan/phpdoc-parser`) [#2502](https://github.com/zephir-lang/zephir/pull/2502)
- Added `ConstructorsCodeGenTest` to verify generated C/H files against reference fixtures [#2482](https://github.com/zephir-lang/zephir/pull/2482)

### Changed
- Refactored string parameter handling to use native `zend_string *` (with companion `zval`) where the parameter isn't mutated, reducing engine round-trips for string params [#2462](https://github.com/zephir-lang/zephir/issues/2462), [#2484](https://github.com/zephir-lang/zephir/pull/2484)
- Refactored type optimizers and added more tests [#2470](https://github.com/zephir-lang/zephir/pull/2470)
- Refactored header compilation logic and updated compatibility checks for PHP 8.5 [#2489](https://github.com/zephir-lang/zephir/pull/2489)
- Bumped `phpstan/phpdoc-parser` from `1.33.0` to `2.3.2` [#2504](https://github.com/zephir-lang/zephir/pull/2504)
- Bumped `squizlabs/php_codesniffer` from `3.13.5` to `4.0.1` [#2473](https://github.com/zephir-lang/zephir/pull/2473)
- Bumped `phpunit/phpunit` from `9.6.32` to `9.6.33` [#2471](https://github.com/zephir-lang/zephir/pull/2471)
- Bumped `codecov/codecov-action` from `3` to `6` [#2472](https://github.com/zephir-lang/zephir/pull/2472), [#2488](https://github.com/zephir-lang/zephir/pull/2488)
- Bumped `actions/checkout` to `6`, `actions/cache` to `5`, `actions/upload-artifact` to `7`, `actions/download-artifact` to `8` [#2474](https://github.com/zephir-lang/zephir/pull/2474), [#2475](https://github.com/zephir-lang/zephir/pull/2475), [#2476](https://github.com/zephir-lang/zephir/pull/2476), [#2477](https://github.com/zephir-lang/zephir/pull/2477), [#2480](https://github.com/zephir-lang/zephir/pull/2480), [#2481](https://github.com/zephir-lang/zephir/pull/2481), [#2464](https://github.com/zephir-lang/zephir/pull/2464)

### Fixed
- Fixed `-Wincompatible-pointer-types` warnings on PHP 8.5+ for `zend_parse_arg_array` (`Z_PARAM_ARRAY`/`Z_PARAM_ARRAY_OR_NULL`) — promoted to error by GCC 14+ [#2462](https://github.com/zephir-lang/zephir/issues/2462), [#2463](https://github.com/zephir-lang/zephir/pull/2463), [#2483](https://github.com/zephir-lang/zephir/pull/2483), [#2508](https://github.com/zephir-lang/zephir/pull/2508)
- Fixed `-Wdiscarded-qualifiers` warnings introduced on PHP 8.5 where `EG(fake_scope)` became `const zend_class_entry *` ([kernel/object.c](kernel/object.c), [kernel/require.c](kernel/require.c), `ZEPHIR_BACKUP_SCOPE`/`ZEPHIR_RESTORE_SCOPE` macros) [#2462](https://github.com/zephir-lang/zephir/issues/2462)
- Fixed memory leak in generated code where native `zend_string *` parameters incremented refcount via `ZVAL_STR_COPY` without a matching decrement on the return path (~3.4 KB per call) [#2500](https://github.com/zephir-lang/zephir/issues/2500), [#2506](https://github.com/zephir-lang/zephir/pull/2506)
- Fixed crash when passing a `char`/`uchar` literal or variable to a method parameter declared as `char`/`uchar` (TypeError in `getResolvedParams`) [#2469](https://github.com/zephir-lang/zephir/issues/2469), [#2507](https://github.com/zephir-lang/zephir/pull/2507)
- Fixed stale `.dep` dependency files causing compilation failures when switching between PHP 8.x versions (now removed during `fullclean`; build order corrected so `make clean` runs before `phpize --clean`) [#2490](https://github.com/zephir-lang/zephir/issues/2490), [#2491](https://github.com/zephir-lang/zephir/pull/2491)
- Fixed missing default values in reflection metadata for method parameters with scalar types [#2457](https://github.com/zephir-lang/zephir/pull/2457)
- Fixed various compiler warnings surfaced by recent GCC/Clang [#2445](https://github.com/zephir-lang/zephir/pull/2445)
- Fixed `backtrace.c` so `PHP_VERSION_ID` is visible (added missing PHP header include)

## [0.19.0] - 2025-05-13
### Added
- Added support of PHP `8.4` [#2440](https://github.com/zephir-lang/zephir/issues/2440), [#2443](https://github.com/zephir-lang/zephir/pull/2443)

### Changed
- Refactor CUFA implementation [#2444](https://github.com/zephir-lang/zephir/pull/2444)

## [0.18.0] - 2024-09-29
### Added
- Added support of PHP `8.3` [#2407](https://github.com/zephir-lang/zephir/issues/2407)
- Added support of multiple return types in stubs
- Changed `PHP_DEBUG` const usage to `ZEND_DEBUG_BUILD`

### Changed
- Changed minimal PHP version to `8.0` [#2407](https://github.com/zephir-lang/zephir/issues/2407)

## [0.17.0] - 2023-02-11
### Added
- Added support of PHP `8.2` [#2255](https://github.com/zephir-lang/zephir/issues/2370)

### Fixed
- Fixed backtrace on alpine [#2397](https://github.com/zephir-lang/zephir/issues/2397)

## [0.16.3] - 2022-09-17
### Fixed
- Fixed segmentation fault on `mixed` return type and PHP 7.4 [#2387](https://github.com/zephir-lang/zephir/issues/2387)

## [0.16.2] - 2022-08-22
### Added
- Added support for `object` return type [#2374](https://github.com/zephir-lang/zephir/issues/2374)

## [0.16.1] - 2022-08-21
### Changed
- Changed usage of `utf8_decode()` function in favour of `mb_convert_encoding()` [#2376](https://github.com/zephir-lang/zephir/issues/2376)

### Fixed
- Fixed generation of `ARG_INFO` for nullable object (`?object`) [#2374](https://github.com/zephir-lang/zephir/issues/2374)

## [0.16.0] - 2022-03-20
### Added
- Added custom list of arg info definition (Phalcon only) [#2341](https://github.com/zephir-lang/zephir/issues/2341)
- Added support for `int|false` return type (PHP >= 8.0 only) [#2338](https://github.com/zephir-lang/zephir/issues/2338)
- Added support of PHP `8.1` [#2255](https://github.com/zephir-lang/zephir/issues/2255)

### Fixed
- Fixed left `null` with `string` condition [#2299](https://github.com/zephir-lang/zephir/issues/2299)
- Improved support of `mixed` type [#2330](https://github.com/zephir-lang/zephir/issues/2330)
- Fixed Interfaces Breaking Child Projects Of Same Root Level Namespace [#2334](https://github.com/zephir-lang/zephir/issues/2334)

## [0.15.2] - 2021-10-24
### Fixed
- Fixed output of `string` type INI in globals [#2312](https://github.com/zephir-lang/zephir/issues/2312)

## [0.15.1] - 2021-10-08
### Fixed
- Fixed support of `string` type in struct globals [#2308](https://github.com/zephir-lang/zephir/issues/2308)

## [0.15.0] - 2021-10-05
### Added
- Added support for `string` type in php.ini [#2280](https://github.com/zephir-lang/zephir/issues/2280)
- Added support for `mixed` [#2276](https://github.com/zephir-lang/zephir/issues/2276)

### Fixed
- Fixed multiple return types in stubs [#2283](https://github.com/zephir-lang/zephir/issues/2283)
- Fixed `bool` return type in stubs [#2272](https://github.com/zephir-lang/zephir/issues/2272)

### Changed
- Removed `.zep` from stubs filenames [#2273](https://github.com/zephir-lang/zephir/issues/2273)

## [0.14.0] - 2021-09-18
### Added
- Added support for `require_once` [#2253](https://github.com/zephir-lang/zephir/issues/2253)

### Changed
- Bumped minimal version of Zephir Parser to `1.4.1`. [#2284](https://github.com/zephir-lang/zephir/issues/2284)

## [0.14.0-beta.3] - 2021-08-06
### Fixed
- Fixed class entry generation of external class [#2261](https://github.com/zephir-lang/zephir/issues/2261)

## [0.14.0-beta.2] - 2021-08-06
### Fixed
- Fixed missing `config/` directory in `zephir.phar` [#2259](https://github.com/zephir-lang/zephir/issues/2259)

## [0.14.0-beta.1] - 2021-08-06
### Fixed
- Fixed nullable dynamic argument definition [#2245](https://github.com/zephir-lang/zephir/issues/2245)

### Changed
- Changed detection of external class entries [#2213](https://github.com/zephir-lang/zephir/issues/2213)

## [0.13.5] - 2021-05-09
### Fixed
- Fixed `zephir build` command [#2240](https://github.com/zephir-lang/zephir/pull/2240)
- Fixed `zephir generate` when processing Closure [#2241](https://github.com/zephir-lang/zephir/pull/2241)
- Fixed stubs generation with variable-length argument [#2239](https://github.com/zephir-lang/zephir/issues/2239)


## [0.13.4] - 2021-04-26
### Fixed
- Fixed cast string from null [#2232](https://github.com/zephir-lang/zephir/issues/2232)
- Fixed strict nullable string type when `null` is passed [#2234](https://github.com/zephir-lang/zephir/issues/2234)


## [0.13.3] - 2021-04-25
### Fixed
- Fixed nullable array [#1094](https://github.com/zephir-lang/zephir/issues/1094)
- Fixed default value detection with Reflection (only PHP 8.0) [#1134](https://github.com/zephir-lang/zephir/issues/1134)
- Updated supported list of class entries for PHP date extension [#2226](https://github.com/zephir-lang/zephir/issues/2226)
- Fixed unset from class property [#1259](https://github.com/zephir-lang/zephir/issues/1259)

### Added
- Added support syntax assign-bitwise operators [#1103](https://github.com/zephir-lang/zephir/issues/1103)


## [0.13.2] - 2021-04-10
### Fixed
- Fixed default value of nullable string parameter [#2180](https://github.com/zephir-lang/zephir/issues/2180)
- Fixed cast of `string` to `int` and `float` [#828](https://github.com/zephir-lang/zephir/issues/828)
- Fix `uint` cast to `unsigned int` in function params [#812](https://github.com/zephir-lang/zephir/issues/812)
- Fixed `null` strict check when variable is `string` type [#2186](https://github.com/zephir-lang/zephir/issues/2186)


## [0.13.1] - 2021-03-31
### Added
- Added jobs `-j, --jobs` option for `zephir compile` [#2174](https://github.com/zephir-lang/zephir/issues/2174)

### Fixed
- Fixed not used arginfo for interface static method without parameters (PHP `>= 8.0` only) [#2178](https://github.com/zephir-lang/zephir/pull/2178)
- Fixed `zephir install` command [#2175](https://github.com/zephir-lang/zephir/issues/2175)


## [0.13.0] - 2021-03-25
### Added
- Added support of PHP `8.0` [#2111](https://github.com/zephir-lang/zephir/pull/2111), [#2165](https://github.com/zephir-lang/zephir/pull/2165)

### Changed
- Dropped support of PHP `< 7.4` versions [#2111](https://github.com/zephir-lang/zephir/pull/2111)
- Removed call of `generate` command inside `compile` call [#2150](https://github.com/zephir-lang/zephir/pull/2150)
- Removed call of `compile` command inside `install` call [#2150](https://github.com/zephir-lang/zephir/pull/2150)

### Fixed
- Fixed parameters type detection in methods/functions (PHP `>= 8.0` only)
- Fixed not adding all build directories [#2144](https://github.com/zephir-lang/zephir/pull/2144)


## [0.12.21] - 2021-03-05
### Fixed
- Fixed path separators in generated `config.m4` file on Windows [#2153](https://github.com/zephir-lang/zephir/issues/2153)


## [0.12.20] - 2020-12-16
### Added
- Added supports void type return value for stubs
  [phalcon/ide-stubs#50](https://github.com/phalcon/ide-stubs/pull/50)
  [#1977](https://github.com/zephir-lang/zephir/issues/1977)

### Fixed
- Fixed missing kernel directory at build time [ice/framework#271](https://github.com/ice/framework/issues/271)
- Fixed stubs generation for case with array declaration with square brackets in params
- Fixed parameters positioning for `implode()` php function [#2120](https://github.com/zephir-lang/zephir/issues/2120)


## [0.12.19] - 2020-05-13
### Fixed
- Fixed duplicate definition with GCC 10
  [ice/framework#266](https://github.com/ice/framework/pull/266)
- Fixed initialization of object properties with default values when
  the object is an instance of a child class
  [#2089](https://github.com/zephir-lang/zephir/issues/2089)

### Changed
- Improved stubs generation for methods which may return object or null
  [#2092](https://github.com/zephir-lang/zephir/issues/2092)


## [0.12.18] - 2020-04-25
### Fixed
- In some cases for C "control characters" aren't properly escaped
  [#2065](https://github.com/zephir-lang/zephir/issues/2065)
- Zephir ignored property visibility and has not thrown error when setting
  private/protected properties in scope that shouldn't intend for it
  [#2078](https://github.com/zephir-lang/zephir/pull/2078),
  [phalcon/cphalcon#14810](https://github.com/phalcon/cphalcon/issues/14810),
  [phalcon/cphalcon#14766](https://github.com/phalcon/cphalcon/issues/14766)


## [0.12.17] - 2020-02-14
### Fixed
- On some platforms special alpha characters aren't correctly escaped.
  [#2058](https://github.com/zephir-lang/zephir/pull/2058)

### Changed
- Changed the internal DI environment mode when compile PHAR
  [#2049](https://github.com/zephir-lang/zephir/pull/2049)


## [0.12.16] - 2020-01-16
### Fixed
- Do not dump config file if config was changed.
  Usually we need dump configuration exactly once - at project initialization.
  There are no needs to dump it for every config change. Also, this patch
  removes `Config::$changed` variable that is no longer needed
  [#2035](https://github.com/zephir-lang/zephir/pull/2035)
- Use a different path for the Kernel cache if possible.
  This patch fixes a cache collision issue. The issue is after creating the
  cache and filling it with a project-specific configuration, there is no
  way to invalidate it. Any next project will use the same Kernel cache and
  the same Kernel configuration (if any).
  [#2036](https://github.com/zephir-lang/zephir/pull/2036)
- Fixed `-V` CLI flag purpose. Initially it was designed to disable verbose mode
  on the fly, e.g. to override project configuration for a single Zephir pass.
  This behavior was returned back.
- Fix increment array elements operation
  [#2020](https://github.com/zephir-lang/zephir/issues/2020)
- Fixed compound addition and subtraction assignment operators for static properties
  [#2038](https://github.com/zephir-lang/zephir/issues/2038)

### Changed
- Improved type hint for arrays when generating stubs
  [#2026](https://github.com/zephir-lang/zephir/issues/2026)


## [0.12.15] - 2019-12-12
### Removed
- Removed `uint` typedef usage


## [0.12.14] - 2019-12-11
### Removed
- Removed `zend_uint` typedef usage


## [0.12.13] - 2019-12-08
### Fixed
- Fixed PHP 7.4 support for macOS [phalcon/cphalcon#14577](https://github.com/phalcon/cphalcon/issues/14577)

### Removed
- Removed `uint` and `ulong` typedefs usage


## [0.12.12] - 2019-11-25
### Added
- Option to set banner for stubs generator
  [#1987](https://github.com/zephir-lang/zephir/1987)

### Fixed
- Calling object methods from static context yields segmentation fault when
  `internal-call-transformation` is set to `TRUE`
  [#2000](https://github.com/zephir-lang/zephir/issues/2000)
- Certain method calls fail when called from static context when
  `internal-call-transformation` is set to `TRUE`
  [#2005](https://github.com/zephir-lang/zephir/issues/2005)
- Method context loses track of `this` after calling static method when
  `internal-call-transformation` is set to `TRUE`
  [#2007](https://github.com/zephir-lang/zephir/issues/2007)
- Fixed incorrect stubs generation for return type hint
  [#1990](https://github.com/zephir-lang/zephir/issues/1990)
- Fixed incorrect stubs generation for classes in the same namespace
  [#2016](https://github.com/zephir-lang/zephir/issues/2016)


## [0.12.11] - 2019-11-02
### Fixed
- Fixed arithmetical operations with `zvals` which stores `double` numbers
- Fixed updating static variables in the loop which represents  `double` and
  `integer` data types [#1494](https://github.com/zephir-lang/zephir/issues/1494)
- Fixed casting char into another of a different type
  [#1988](https://github.com/zephir-lang/zephir/issues/1988)
- Fixed `internal` methods definition when `internal-call-transformation` is enabled
  [#1956](https://github.com/zephir-lang/zephir/issues/1956)
- Fixed aliases using in the `use` statement when generating stubs
  [#1986](https://github.com/zephir-lang/zephir/issues/1986)
- Fixed incorrect namespace on type hinted return when generating API docs
  [#1229](https://github.com/zephir-lang/zephir/issues/1229)


## [0.12.10] - 2019-10-19
### Fixed
- Fixed incorrect behavior in `zephir_get_global` if `zval` reference count <= 1
  [#1961](https://github.com/zephir-lang/zephir/issues/1961)

### Removed
- Removed `--vernum` option from the help for regular commands
- Removed `void` from the return type hint in the generated stubs
  [#1977](https://github.com/zephir-lang/zephir/issues/1977)
- Remove no longer supported `TSRMLS_CC` usage
  [#1865](https://github.com/zephir-lang/zephir/issues/1865)

### Changed
- Disabled PHP warnings for PHP >= 7.3.0 to be able correct work with lowest versions of dependencies
  [zendframework/zend-code#160](https://github.com/zendframework/zend-code/issues/160)
- Introduced support of multi line `@param` body for generated stubs
  [#1968](https://github.com/zephir-lang/zephir/issues/1968)


## [0.12.9] - 2019-10-14
### Added
- Added a single hyphen version of `dumpversion` option (just `-dumpversion`)
- Added `--vernum` option to print compiler version as integer

### Fixed
- Create local `.zephir` only when necessary
- Fixed IDE stubs generation [#1778](https://github.com/zephir-lang/zephir/issues/1778)
- Fixed segfault on cast `IS_UNDEF` to array
  [#1941](https://github.com/zephir-lang/zephir/issues/1941)
- Disables some regression changes introduced in the version `0.12.5`
  [#1941 (comment)](https://github.com/zephir-lang/zephir/issues/1941#issuecomment-538654340)
- Fixed memory leak on update array [#1937](https://github.com/zephir-lang/zephir/issues/1937)
- Fixed IDE stubs generation for classes that extends base classes
  [#1907](https://github.com/zephir-lang/zephir/issues/1907)
- Proper escape slashes in strings [#1495](https://github.com/zephir-lang/zephir/issues/1495)

### Changed
- Print warning during the code generation if the `timecop` extension was detected
  [#1950](https://github.com/zephir-lang/zephir/issues/1950)
- Improved error handling to not print PHP stack traces if `ZEPHIR_DEBUG` is not set

### Removed
- Removed no longer used `zephir_dtor` macro


## [0.12.8] - 2019-10-03
### Fixed
- Fixed `zephir_preg_match` to use `ZVAL_NULL` instead of `ZEPHIR_NULL`
  [#1946](https://github.com/zephir-lang/zephir/issues/1946)
- Fixed `Extension\InternalClassesTest` test to be able run full test suite
  without Phalcon [#1949](https://github.com/zephir-lang/zephir/issues/1949)


## [0.12.7] - 2019-10-03
### Fixed
- Fixed regression introduced in `0.12.5` for those users who doesn't use
  bundled `ext/pcre/php_pcre.h`
  [#1944](https://github.com/zephir-lang/zephir/issues/1944)
  [#1940](https://github.com/zephir-lang/zephir/issues/1940)
- Fixed sitemap API generator [#1940](https://github.com/zephir-lang/zephir/issues/1940)


## [0.12.6] - 2019-10-03
### Fixed
- Fixed regression introduced in `0.12.5` for those users who doesn't use
  bundled `ext/json/php_json.h` [#1940](https://github.com/zephir-lang/zephir/issues/1940)


## [0.12.5] - 2019-10-02
### Changed
- Update `zend_update_static_property` to be compatible with PHP >= 7.3
  [#1904](https://github.com/zephir-lang/zephir/issues/1904)
- Improved error handling

### Fixed
- Fixed IDE stubs generation to properly generate return type hint for `var | null`
  [#1922](https://github.com/zephir-lang/zephir/issues/1922)
- Fixed updating Super Globals [#1917](https://github.com/zephir-lang/zephir/issues/1917)
- Fixed casting variables to array [#1923](https://github.com/zephir-lang/zephir/issues/1923)
- Fixed work with constant which are not present
  [#1928](https://github.com/zephir-lang/zephir/issues/1928)
- Fixed access to Super Globals
  [#1934](https://github.com/zephir-lang/zephir/issues/1934),
  [phalcon/cphalcon#14426](https://github.com/phalcon/cphalcon/issues/14426)


## [0.12.4] - 2019-09-22
### Fixed
- Fixed install template


## [0.12.3] - 2019-09-22
### Fixed
- Fixed concatenation support of strings with `double` numbers
  [#1893](https://github.com/zephir-lang/zephir/issues/1893)
- Fixed 'void' return type hint being ignored
  [#1908](https://github.com/zephir-lang/zephir/issues/1908)
- Fixed updating array properties
  [#1915](https://github.com/zephir-lang/zephir/issues/1915)


## [0.12.2] - 2019-08-05
### Added
- Introduced initial ability to generate `zend_module_deps`
  [#1900](https://github.com/zephir-lang/zephir/pull/1900),
  [phalcon/cphalcon#13702](https://github.com/phalcon/cphalcon/issues/13702),
  [phalcon/cphalcon#13794](https://github.com/phalcon/cphalcon/pull/13794)

### Changed
- Write errors compiler to stderr if available


## [0.12.1] - 2019-07-30
### Added
- Added initial bash completion support (see `zephir-autocomplete` file)

### Changed
- Use local memory management
  [#1859](https://github.com/zephir-lang/zephir/pull/1859),
  [#1880](https://github.com/zephir-lang/zephir/pull/1880)
- Rephrase help strings for common compiler options

### Removed
- Remove HAVE_SPL usage
  [phalcon/cphalcon#14215](https://github.com/phalcon/cphalcon/pull/14215)
- Remove not used redundant command line options
- Cleaning up redundant CLI options

### Fixed
- Fixed segfault when auto-loading class with syntax error
  [#1885](https://github.com/zephir-lang/zephir/issues/1885)
- Optimize memory usage [#1882](https://github.com/zephir-lang/zephir/pull/1882)
- Fixed modifying array values in loops
  [#1879](https://github.com/zephir-lang/zephir/issues/1879)


## [0.12.0] - 2019-06-20
### Added
- Added initial support of "use" keyword in closures
  [#888](https://github.com/zephir-lang/zephir/issues/888),
  [#1848](https://github.com/zephir-lang/zephir/issues/1848)
  [#1860](https://github.com/zephir-lang/zephir/issues/1860)

### Removed
- PHP 5.x is no longer supported

### Changed
- The minimal Zephir Parser version is 1.3.0

### Fixed
- Fixed CLI runner for Windows
  [#1857](https://github.com/zephir-lang/zephir/pull/1857)
- Fixed segfault with fetching and opcache
  [#1855](https://github.com/zephir-lang/zephir/issues/1855)
- Extended classes can't access their private variables
  [#1851](https://github.com/zephir-lang/zephir/issues/1851)
- Incorrect usage of `zend_declare_class_constant_ex`
  [phalcon/cphalcon#14160](https://github.com/phalcon/cphalcon/issues/14160),
  https://bugs.php.net/bug.php?id=78121
- Incorrect implementation of ArrayAccess methods
  [#1871](https://github.com/zephir-lang/zephir/pull/1871)
- Fixed exception on call undefined method
  [#1863](https://github.com/zephir-lang/zephir/issues/1863)


## [0.11.12] - 2019-03-24
### Fixed
- Compilation error for instanceof [#1828](https://github.com/zephir-lang/zephir/issues/1828)
- Fixed `array_shift` behavior to mimicry PHP's "change by reference"
  [#1831](https://github.com/zephir-lang/zephir/issues/1831)
- Fixed reference counting while changing object's properties that are arrays
  [#1833](https://github.com/zephir-lang/zephir/pull/1833)


## [0.11.11] - 2019-02-26
### Fixed
- Objects are not traversable with `foreach`
  [#1818](https://github.com/zephir-lang/zephir/issues/1818)
  [#1595](https://github.com/zephir-lang/zephir/issues/1595)
- Recursion for array_push on PHP 7 [#1140](https://github.com/zephir-lang/zephir/issues/1140)
- Invalid array initialization [#1159](https://github.com/zephir-lang/zephir/issues/1159)


## [0.11.10] - 2019-02-23
### Changed
- Moved internal cache and logs to the user's home directory.
  - On macOS Zephir will use `XDG` if it is possible, otherwise `$HOME/Library`
  - On Windows Zephir will use `LOCALAPPDATA` if it is possible, otherwise home dir as a base path
  - In any other cases, e.g. Linux, BSD and so on, Zephir will use `XDG`if it is possible,
    otherwise `$HOME/.local` and `$HOME/.cache`
- Per project cache used for temporary operations was moved to `%CWD%/.zephir/%VERSION%`
  where `%CWD%` is the current working directory and `%VERSION%` is the current Zephir version e.g. `0.11.10-4a825b8`

### Fixed
- Array of object as return type is reported to PHP as type, not array
  [#1779](https://github.com/zephir-lang/zephir/issues/1779)
- Use namespace as a prefix for ini name [#1604](https://github.com/zephir-lang/zephir/issues/1604)
- Fixed calling anonymous functions by resolving context [#1751](https://github.com/zephir-lang/zephir/issues/1751)
- Fixed a bug when accessing super-global variables that do not exist (only for PHP >= 7.0 )
  [#1775](https://github.com/zephir-lang/zephir/issues/1775)
- Fixed incorrect behavior during work with ArrayAccess [#1061](https://github.com/zephir-lang/zephir/issues/1061),
  [#1400](https://github.com/zephir-lang/zephir/issues/1400)


## [0.11.9] - 2019-01-15
- Fixed `zend_closure` declaration to reflect PHP 7.3 changes


## [0.11.8] - 2018-12-01
### Fixed
- Fixed compilation error with inheritance of prototype interfaces
  [#1758](https://github.com/zephir-lang/zephir/issues/1758)
- Fixed compilation error when a new file is added or removed to the project
  [#1776](https://github.com/zephir-lang/zephir/issues/1776)


## [0.11.7] - 2018-11-27
### Changed
- The cache directory, formerly known as `.temp`, used for temporary operations was moved to
  the new `.zephir` directory. The algorithm for calculating cache path is as follows:
  `%CWD%/.zephir/%HASH%/cache/IR` where `%CWD%` is the current working directory and `%HASH%`
  means a hash calculated from the current Zephir version, environment and configuration
- The compiler's messages was divided into streams. Thus, now it is possible to redirect compiler's
  output as follows: `zephir generate 2> errors.log 1> /dev/null`
- Fixed type hints for scalar arguments for PHP < 7.2
  [#1658](https://github.com/zephir-lang/zephir/pull/1658)
- Coloring the compiler messages in the terminal is temporarily disabled

### Fixed
- Fixed incorrect behavior of `func_get_arg` and `func_get_args` functions for PHP 7.3


## [0.11.6] - 2018-11-19
### Fixed
- Fixed incorrect behavior of `require` statement for ZendEngine3
  [#1621](https://github.com/zephir-lang/zephir/issues/1621)
  [#1403](https://github.com/zephir-lang/zephir/issues/1403)
  [#1428](https://github.com/zephir-lang/zephir/pull/1428)


## [0.11.4] - 2018-11-18
### Added
- Introduced a brand new CLI interface
- The preferred method of installation is to use the Zephir PHAR
  which can be downloaded from the most recent Github Release
- Added `--no-dev` option to force building the extension in production mode
  [#1520](https://github.com/zephir-lang/zephir/issues/1520)
- Zephir development mode will be enabled silently if your PHP binary was compiled in
  a debug configuration [#1520](https://github.com/zephir-lang/zephir/issues/1520)
- Added missed CLI option `--export-classes` to flag whether classes must be exported.
  If export-classes is enabled all headers are copied to `include/php/ext`.

### Fixed
- Fixed regression introduced in the 0.10.12 related to `require` file using protocols
  [#1713](https://github.com/zephir-lang/zephir/issues/1713)


## [0.11.3] - 2018-11-13
### Changed
- Remove legacy installers and provide a common way to install Zephir
  [#1714](https://github.com/zephir-lang/zephir/issues/1714). Supported installation strategies are:
  - Install as a global application (using `composer global require`)
  - Install as a PHAR file. (this feature currently in the testing phase and not released officially)
  - Install as a Git clone (using `git clone` and `composer install` inside cloned project)
  - Install as a project's dependency (using `composer require`)


## [0.11.2] - 2018-11-11
### Added
- Introduced an ability to pack project into one `zephir.phar` file (for PHP 7.1 and later)

### Changed
- Composer now is a mandatory dependency
- Improved Zephir's Compiler error reporting

### Removed
- PHP 5.5 no longer supported

### Fixed
- Correct return types hint check


## [0.11.1] - 2018-10-19
### Added
- Initial support of PHP 7.3 (ported from the 0.10.x branch)


## [0.11.0] - 2018-08-05
### Added
- Add type hints for scalar arguments and return values in ZendEngine 3
  [1656](https://github.com/zephir-lang/zephir/issues/1656)


## [0.10.14] - 2018-11-20
### Fixed
- Fixed incorrect behavior of `func_get_arg` and `func_get_args` functions for PHP 7.3
  (backported from the upstream)


## [0.10.13] - 2018-11-18
### Fixed
- Fixed regression introduced in the 0.10.12 related to `require` file using protocols
  [#1713](https://github.com/zephir-lang/zephir/issues/1713) (backported from the upstream)
- Fixed incorrect behavior of `require` statement for ZendEngine3
  [#1621](https://github.com/zephir-lang/zephir/issues/1621)
  [#1403](https://github.com/zephir-lang/zephir/issues/1403)
  [#1428](https://github.com/zephir-lang/zephir/pull/1428)
  (backported from the upstream)


## [0.10.12] - 2018-10-19
### Added
- Initial support of PHP 7.3


## [0.10.11] - 2018-08-05
### Added
- Allow extension to be loaded prior to the tests

### Fixed
- Fixed [Copy-On-Write](https://en.wikipedia.org/wiki/Copy-on-write) violation for arrays zvals
- Fixed some testing settings
  [5deb64a](https://github.com/zephir-lang/zephir/commit/5deb64a8a1c7c18d45ce1a5a55667c499e2c284f)
- Fixed casting resource to int (only ZendEngine 3)
  [#1524](https://github.com/zephir-lang/zephir/issues/1524)


[Unreleased]: https://github.com/zephir-lang/zephir/compare/1.0.0...HEAD
[1.0.0]: https://github.com/zephir-lang/zephir/compare/0.23.0...1.0.0
[0.23.0]: https://github.com/zephir-lang/zephir/compare/0.22.0...0.23.0
[0.22.0]: https://github.com/zephir-lang/zephir/compare/0.21.0...0.22.0
[0.21.0]: https://github.com/zephir-lang/zephir/compare/0.20.1...0.21.0
[0.20.1]: https://github.com/zephir-lang/zephir/compare/0.20.0...0.20.1
[0.20.0]: https://github.com/zephir-lang/zephir/compare/0.19.0...0.20.0
[0.19.0]: https://github.com/zephir-lang/zephir/compare/0.18.0...0.19.0
[0.18.0]: https://github.com/zephir-lang/zephir/compare/0.17.0...0.18.0
[0.17.0]: https://github.com/zephir-lang/zephir/compare/0.16.3...0.17.0
[0.16.3]: https://github.com/zephir-lang/zephir/compare/0.16.2...0.16.3
[0.16.2]: https://github.com/zephir-lang/zephir/compare/0.16.1...0.16.2
[0.16.1]: https://github.com/zephir-lang/zephir/compare/0.16.0...0.16.1
[0.16.0]: https://github.com/zephir-lang/zephir/compare/0.15.2...0.16.0
[0.15.2]: https://github.com/zephir-lang/zephir/compare/0.15.1...0.15.2
[0.15.1]: https://github.com/zephir-lang/zephir/compare/0.15.0...0.15.1
[0.15.0]: https://github.com/zephir-lang/zephir/compare/0.14.0...0.15.0
[0.14.0]: https://github.com/zephir-lang/zephir/compare/0.14.0-beta.3...0.14.0
[0.14.0-beta.3]: https://github.com/zephir-lang/zephir/compare/0.14.0-beta.2...0.14.0-beta.3
[0.14.0-beta.2]: https://github.com/zephir-lang/zephir/compare/0.14.0-beta.1...0.14.0-beta.2
[0.14.0-beta.1]: https://github.com/zephir-lang/zephir/compare/0.13.5...0.14.0-beta.1
[0.13.5]: https://github.com/zephir-lang/zephir/compare/0.13.4...0.13.5
[0.13.4]: https://github.com/zephir-lang/zephir/compare/0.13.3...0.13.4
[0.13.3]: https://github.com/zephir-lang/zephir/compare/0.13.2...0.13.3
[0.13.2]: https://github.com/zephir-lang/zephir/compare/0.13.1...0.13.2
[0.13.1]: https://github.com/zephir-lang/zephir/compare/0.13.0...0.13.1
[0.13.0]: https://github.com/zephir-lang/zephir/compare/0.12.21...0.13.0
[0.12.21]: https://github.com/zephir-lang/zephir/compare/0.12.20...0.12.21
[0.12.20]: https://github.com/zephir-lang/zephir/compare/0.12.19...0.12.20
[0.12.19]: https://github.com/zephir-lang/zephir/compare/0.12.18...0.12.19
[0.12.18]: https://github.com/zephir-lang/zephir/compare/0.12.17...0.12.18
[0.12.17]: https://github.com/zephir-lang/zephir/compare/0.12.16...0.12.17
[0.12.16]: https://github.com/zephir-lang/zephir/compare/0.12.15...0.12.16
[0.12.15]: https://github.com/zephir-lang/zephir/compare/0.12.14...0.12.15
[0.12.14]: https://github.com/zephir-lang/zephir/compare/0.12.13...0.12.14
[0.12.13]: https://github.com/zephir-lang/zephir/compare/0.12.12...0.12.13
[0.12.12]: https://github.com/zephir-lang/zephir/compare/0.12.11...0.12.12
[0.12.11]: https://github.com/zephir-lang/zephir/compare/0.12.10...0.12.11
[0.12.10]: https://github.com/zephir-lang/zephir/compare/0.12.9...0.12.10
[0.12.9]: https://github.com/zephir-lang/zephir/compare/0.12.8...0.12.9
[0.12.8]: https://github.com/zephir-lang/zephir/compare/0.12.7...0.12.8
[0.12.7]: https://github.com/zephir-lang/zephir/compare/0.12.6...0.12.7
[0.12.6]: https://github.com/zephir-lang/zephir/compare/0.12.5...0.12.6
[0.12.5]: https://github.com/zephir-lang/zephir/compare/0.12.4...0.12.5
[0.12.4]: https://github.com/zephir-lang/zephir/compare/0.12.3...0.12.4
[0.12.3]: https://github.com/zephir-lang/zephir/compare/0.12.2...0.12.3
[0.12.2]: https://github.com/zephir-lang/zephir/compare/0.12.1...0.12.2
[0.12.1]: https://github.com/zephir-lang/zephir/compare/0.12.0...0.12.1
[0.12.0]: https://github.com/zephir-lang/zephir/compare/0.11.12...0.12.0
[0.11.12]: https://github.com/zephir-lang/zephir/compare/0.11.11...0.11.12
[0.11.11]: https://github.com/zephir-lang/zephir/compare/0.11.10...0.11.11
[0.11.10]: https://github.com/zephir-lang/zephir/compare/0.11.9...0.11.10
[0.11.9]: https://github.com/zephir-lang/zephir/compare/0.11.8...0.11.9
[0.11.8]: https://github.com/zephir-lang/zephir/compare/0.11.7...0.11.8
[0.11.7]: https://github.com/zephir-lang/zephir/compare/0.11.6...0.11.7
[0.11.6]: https://github.com/zephir-lang/zephir/compare/0.11.4...0.11.6
[0.11.4]: https://github.com/zephir-lang/zephir/compare/0.11.3...0.11.4
[0.11.3]: https://github.com/zephir-lang/zephir/compare/0.11.2...0.11.3
[0.11.2]: https://github.com/zephir-lang/zephir/compare/0.11.1...0.11.2
[0.11.1]: https://github.com/zephir-lang/zephir/compare/0.11.0...0.11.1
[0.11.0]: https://github.com/zephir-lang/zephir/compare/0.10.14...0.11.0
[0.10.14]: https://github.com/zephir-lang/zephir/compare/0.10.13...0.10.4
[0.10.13]: https://github.com/zephir-lang/zephir/compare/0.10.12...0.10.13
[0.10.12]: https://github.com/zephir-lang/zephir/compare/0.10.11...0.10.12
[0.10.11]: https://github.com/zephir-lang/zephir/compare/0.10.10...0.10.11
