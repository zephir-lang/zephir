# `<Ns>\Buffer` — usage

A `Buffer` is a fixed-size, contiguous array of C scalars — either `double` or `int` — held inside
an ordinary refcounted PHP object. A PHP array of a million floats costs a zval plus a hash slot per
element, and the only way to hand it to a C numeric library is to walk it into a scratch buffer and
walk the result back out. A `Buffer` is the place to keep that data *between* operations instead:
the elements live in one `emalloc`'d C array, C code reads and writes them through a raw pointer,
and a PHP array is materialised only at the boundary, by `toArray()`.

**The governing rule:** the payoff scales with the length of the chain, not with a single call.
Converting in, doing one operation, and converting straight back out is usually a net loss. See
[Performance](./performance.md).

Added in [#2721](https://github.com/zephir-lang/zephir/issues/2721).

## Turning it on

The class is **off by default** — an extension never gains a class it did not ask for. Opt in from
your project's `config.json` with the top-level `kernel-classes` section:

```json
{
    "namespace": "myext",
    "kernel-classes": {
        "buffer": true
    }
}
```

The default lives in `src/Config.php:174`. Zephir's own stub project turns it on at
`config.json:185`.

Two things here are easy to get wrong, and both fail **silently**:

- The switch is a strict identity check against `true` — `src/Compiler.php:1103` reads
  `if (true !== $this->config->get('buffer', 'kernel-classes'))`. JSON `1`, `"true"` or `"yes"` all
  leave the class compiled out with no warning.
- A top-level section from `config.json` **replaces** the corresponding default outright, it does
  not merge into it (`Config::offsetSet()`, `src/Config.php:381`). Listing one kernel class in
  `kernel-classes` therefore drops the defaults of every other key in that section.

Turning the switch on changes the **generated** extension, not just the compiled binary, so
regenerate before rebuilding:

```bash
php zephir fullclean
php zephir build
```

Confirm it took by looking for the two defines in the generated header:

```bash
grep ZEPHIR_BUFFER ext/php_myext.h
```

```c
#define ZEPHIR_BUFFER_ENABLED 1
#define ZEPHIR_BUFFER_NAMESPACE "Myext"
```

## What the class is called

The class is registered as `<RootNamespace>\Buffer`, where the root namespace is taken
**proper-cased from a compiled class**, not from the lower-case `namespace` key in `config.json`
(`Compiler::properCaseRootNamespace()`, `src/Compiler.php:1132`). This repository's own stub
extension declares `"namespace": "stub"` and gets `Stub\Buffer` (`ext/php_stub.h:24`).

If your project already declares a class with that name, the build stops with a `CompilerException`
rather than shadowing it (`src/Compiler.php:1112`):

```
Class "Myext\Buffer" collides with the compiler-provided buffer class registered by
`kernel-classes.buffer`. Rename the class, or turn the option off in config.json.
```

## From Zephir

The compiler has no definition to check a `<Buffer>` type hint against — it is a hand-written kernel
class, not a compiled `.zep` file — so buffer parameters are declared `var`. Indexing still takes
the kernel fast path; the speed does not depend on the hint.

```zephir
namespace Stub;

class BufferOps
{
    public function readAt(var buf, int index)
    {
        return buf[index];
    }

    public function writeAt(var buf, int index, var value) -> void
    {
        let buf[index] = value;
    }

    public function has(var buf, int index) -> bool
    {
        return isset buf[index];
    }

    public function size(var buf) -> int
    {
        return count(buf);
    }
}
```

A read loop — this is the shape the class exists for (`stub/bufferops.zep:33`):

```zephir
    public function sum(var buf) -> double
    {
        double total = 0.0;
        int i = 0, n = 0;

        let n = count(buf);

        while i < n {
            let total += (double) buf[i];
            let i++;
        }

        return total;
    }
```

And a read-modify-write loop (`stub/bufferops.zep:48`):

```zephir
    public function scale(var buf, double factor) -> void
    {
        int i = 0, n = 0;

        let n = count(buf);

        while i < n {
            let buf[i] = ((double) buf[i]) * factor;
            let i++;
        }
    }
```

`buf[i]`, `let buf[i] = v` and `isset buf[i]` are the three operations that bypass
`offsetGet()`/`offsetSet()` entirely. `count(buf)` does not need the fast path — it goes through the
`count_elements` object handler, which is already a field read.

## From PHP

```php
use Stub\Buffer;

$buffer = new Buffer(3);              // 3 doubles, zero-filled
$buffer[0] = 1.5;
$buffer[1] = -2.5;

$buffer[0];                           // 1.5
count($buffer);                       // 3
$buffer->type();                      // Buffer::TYPE_DOUBLE
$buffer->toArray();                   // [1.5, -2.5, 0.0]

$ints = new Buffer(2, Buffer::TYPE_LONG);
$ints[0] = 42;
$ints->toArray();                     // [42, 0]

$fromArray = Buffer::fromArray([1.0, 2.0, 3.0]);
$fromArray->fill(7.5);
$fromArray->toArray();                // [7.5, 7.5, 7.5]
```

`fromArray()` is positional: keys are discarded and values are taken in iteration order.

```php
Buffer::fromArray(['b' => 2.0, 'a' => 1.0, 7 => 3.0])->toArray();   // [2.0, 1.0, 3.0]
```

## Choosing the element type

Two kinds, fixed at construction and never changed afterwards:

| Constant | C type | `toArray()` yields |
| --- | --- | --- |
| `Buffer::TYPE_DOUBLE` (default) | `double` | `float` |
| `Buffer::TYPE_LONG` | `int` (`zend_long`) | `int` |

A write converts with the engine's own cast — `zval_get_double()` or `zval_get_long()`
(`kernel/buffer.c:104`) — so an element ends up holding exactly what `(float)` or `(int)` would have
produced, PHP's own notices included. There is **no type check and no `TypeError`**:

```php
$doubles = new Buffer(1);
$doubles[0] = '2.5';                  // 2.5
$doubles[0] = true;                   // 1.0
$doubles[0] = null;                   // 0.0

$longs = new Buffer(1, Buffer::TYPE_LONG);
$longs[0] = 3.9;                      // 3
$longs[0] = '12abc';                  // 12, with PHP's usual notice
```

If you need rejection rather than coercion, validate before writing.

## Traps

These are the places where a `Buffer` does not behave like a PHP array. None of them are bugs; all
of them follow from the elements being raw C scalars rather than zvals.

**An element is not an lvalue.** `read_dimension` returns the value in a temporary, never a pointer
into the buffer, because the elements are raw C scalars with no zval to point at
(`kernel/buffer.c:306`). Compound assignment still works, because the engine implements it on an
object as a read followed by a write:

```php
$buffer[0] += 5.0;                    // works: offsetGet, add, offsetSet
```

But anything that needs a *reference* to the element does not, and raises PHP's standard notice
instead — *Indirect modification of overloaded element of `<Ns>\Buffer` has no effect*:

```php
$buffer[0]++;                         // notice; $buffer[0] unchanged
--$buffer[0];                         // notice; $buffer[0] unchanged
$ref =& $buffer[0];                   // notice; writes through $ref never land
byReference($buffer[0]);              // notice; any by-reference parameter

$buffer[0] = $buffer[0] + 1.0;        // do this instead of ++
```

This is not specific to `Buffer` — it is what any `ArrayAccess` object does when it does not hand
back a reference, which is why `tests/Extension/BufferTest.php:260` asserts compound assignment, and
`tests/Extension/BufferTest.php:318` the reference-taking forms, against a plain `ArrayAccess` class
rather than against `SplFixedArray` (whose elements really are zvals).

In Zephir the same rule applies: read into a local, compute, write back — which is what `scale()`
above does.

**`foreach` materialises the whole buffer.** `getIterator()` builds an `ArrayIterator` over a full
PHP array copy (`kernel/buffer.c:745`). That is deliberate — stepping one element at a time through
the VM is the slow path by definition — but it means `foreach` over a million-element buffer
allocates the million-element array you were trying to avoid. In hot code, index:

```php
for ($i = 0, $n = count($buffer); $i < $n; $i++) {
    // $buffer[$i]
}
```

**Every in-range element is set, including zero.** A buffer holds numbers, and no number reads as
absent, so `isset()` is true for any in-range index on a freshly constructed buffer
(`kernel/buffer.c:386`). This is the one place where the class deliberately disagrees with
`SplFixedArray`, whose slots start out `null`.

```php
$buffer = new Buffer(2);
isset($buffer[0]);                    // true  (SplFixedArray: false)
isset($buffer[2]);                    // false (out of range)
$buffer[9] ?? 'fallback';             // 'fallback', no exception
```

**`unset()` zeroes, it does not remove.** There is no hole to make (`kernel/buffer.c:399`):

```php
$buffer = Buffer::fromArray([1.5, 2.5]);
unset($buffer[0]);
$buffer->toArray();                   // [0.0, 2.5]
count($buffer);                       // still 2
```

**`==` compares nothing useful.** No `compare` handler is installed, so two buffers fall back to the
engine's default object comparison — which compares declared properties, and a buffer has none.
Every buffer therefore compares equal to every other buffer, whatever its contents, length or
element type:

```php
Buffer::fromArray([1.0]) == Buffer::fromArray([2.0]);          // true (!)
Buffer::fromArray([1.0]) == Buffer::fromArray([1.0, 2.0]);     // true (!)

Buffer::fromArray([1.0])->toArray() == Buffer::fromArray([2.0])->toArray();   // false
```

Compare `toArray()` results, or compare element by element. `===` still means identity, so it
behaves as expected.

**`(array)` casts to an empty array.** There is no `cast_object` handler and there are no
properties, so `(array) $buffer` is `[]`. Use `toArray()`. (`var_dump()` does show the elements —
that goes through `get_debug_info`, `kernel/buffer.c:274`.)

**Fixed size, positional, numeric.** No `$buffer[] = $v` append, no string keys, no resizing, and
`fromArray()` discards keys. A `Buffer` is `final` and rejects dynamic properties. For the exact
exception raised in each case, see the [API reference](./api.md).

## Cookbook

**Hold a buffer in a property.** It is an ordinary object: refcounted, storable, garbage-collected.

```zephir
namespace Myext;

class Signal
{
    protected buf;

    public function __construct(int size)
    {
        let this->buf = new \Myext\Buffer(size);
    }

    public function at(int i) -> double
    {
        var buf;

        let buf = this->buf;

        return (double) buf[i];
    }
}
```

Copy the property into a local before a hot loop. A property read returns a fresh temporary each
time; a local does not.

**Convert only at the boundary.** Accept and return PHP arrays in your public API if you must, but
keep the buffer alive across the intermediate steps:

```php
$buf = Buffer::fromArray($input);     // one conversion in
$math->normalize($buf);               // no conversion
$math->scale($buf, 2.0);              // no conversion
$math->clamp($buf, 0.0, 1.0);         // no conversion
return $buf->toArray();               // one conversion out
```

**Serialise and encode.** Both work, and both go through the element list:

```php
json_encode(Buffer::fromArray([1.5, 2.5]));      // '[1.5,2.5]'
unserialize(serialize($buffer))->toArray();      // round-trips type and values
```

`json_encode()` emits a JSON list rather than `{}` because the class implements `JsonSerializable`
(`kernel/buffer.c:762`).

**Clone is a deep copy.** The element array is copied, not shared (`kernel/buffer.c:251`):

```php
$copy = clone $original;
$copy[0] = 99.0;                      // $original[0] unchanged
```

## See also

- [API reference](./api.md) — signatures, exceptions, version-specific behaviour.
- [Performance](./performance.md) — what the win is and how to measure it.
- [Internals](./internals.md) — the C pointer API, for extension authors.
