# `<Ns>\Buffer` — API reference

Everything here is read out of `kernel/buffer.c`. `<Ns>` is your project's proper-cased root
namespace; see [Usage](./usage.md#what-the-class-is-called).

## Synopsis

```php
final class <Ns>\Buffer implements ArrayAccess, Countable, IteratorAggregate, JsonSerializable
{
    public const TYPE_DOUBLE = 1;
    public const TYPE_LONG   = 2;

    public function __construct(int $size, int $type = self::TYPE_DOUBLE);
    public static function fromArray(array $values, int $type = self::TYPE_DOUBLE);

    public function toArray(): array;
    public function type(): int;
    public function count(): int;
    public function fill(mixed $value): void;

    public function offsetExists(mixed $offset): bool;
    public function offsetGet(mixed $offset): mixed;
    public function offsetSet(mixed $offset, mixed $value): void;
    public function offsetUnset(mixed $offset): void;

    public function getIterator(): Traversable;
    public function jsonSerialize(): mixed;
    public function __serialize(): array;
    public function __unserialize(array $data): void;
}
```

Two details the synopsis smooths over:

- `__construct()` and `fromArray()` declare **no return type** in their arg-info
  (`kernel/buffer.c:815`, `:815`).
- The `$type` default is applied in C (`kernel/buffer.c:602`, `:621`), not in arg-info, so
  reflection reports the parameter as optional but shows no default-value expression.

The class is `final` and carries `ZEND_ACC_NO_DYNAMIC_PROPERTIES` (`kernel/buffer.c:889`), so it can
neither be extended nor grow ad-hoc properties.

## Constants

| Constant | Value | Element |
| --- | --- | --- |
| `TYPE_DOUBLE` | `1` | C `double` |
| `TYPE_LONG` | `2` | C `zend_long` |

Declared at `kernel/buffer.c:892`. The kind is fixed at construction and never changes.

## Methods

### `__construct(int $size, int $type = self::TYPE_DOUBLE)`

Allocates `$size` zero-filled elements. `$size === 0` is legal and allocates nothing
(`kernel/buffer.c:84`).

- `$size < 0` → `ValueError`, argument #1.
- `$type` not one of the two constants → `ValueError`, argument #2.

### `static fromArray(array $values, int $type = self::TYPE_DOUBLE)`

Builds a buffer from the **values** of `$values` in iteration order. Keys are discarded — a buffer
is positional (`kernel/buffer.c:452`). Each value is converted with the same cast a write uses.

- `$type` invalid → `ValueError`, argument #2.

### `toArray(): array`

Materialises the elements as a packed PHP list. O(n), and the main boundary cost — see
[Performance](./performance.md).

### `type(): int`

Returns `TYPE_DOUBLE` or `TYPE_LONG`.

### `count(): int`

Element count. `count($buffer)` reaches the same value through the `count_elements` object handler
(`kernel/buffer.c:295`) without a method call.

### `fill(mixed $value): void`

Converts `$value` once, then writes it to every element (`kernel/buffer.c:663`).

### `offsetExists` / `offsetGet` / `offsetSet` / `offsetUnset`

The `ArrayAccess` surface; `$buffer[$i]` reaches the same code through the dimension handlers.
Semantics:

- `offsetExists()` is true for **every in-range index**, including one holding zero.
- `offsetGet()` returns `float` or `int` according to the buffer's kind.
- `offsetSet()` coerces; it never rejects on type.
- `offsetUnset()` writes the zero element — it does not shrink the buffer or create a hole.

### `getIterator(): Traversable`

Returns an `ArrayIterator` over a **fully materialised** copy of the elements
(`kernel/buffer.c:745`). There is no lazy iterator: stepping through the VM one element at a time is
the slow path the class exists to avoid, so `foreach` is an O(n)-allocation convenience, not a fast
path.

### `jsonSerialize(): mixed`

Returns the element list, so `json_encode($buffer)` emits `[1.5,2.5]`. Without this the engine would
serialise an object with no properties as `{}` and drop every element (`kernel/buffer.c:760`).

### `__serialize(): array` / `__unserialize(array $data): void`

The payload is a two-element list: `[kind, elements]` (`kernel/buffer.c:769`). `__unserialize()`
validates it and throws on anything else.

## Offset conversion

Any offset is converted to an index by `zephir_buffer_offset_to_long()` (`kernel/buffer.c:150`),
which follows php-src's own `spl_offset_convert_to_long()`:

| Offset | Becomes |
| --- | --- |
| `int` | itself |
| numeric string (`'1'`) | that integer |
| `float` | truncated toward zero |
| `false` / `true` | `0` / `1` |
| reference | dereferenced, then re-examined |
| resource | its handle (deprecated from PHP 8.1) |
| anything else | illegal — see below |

The range check is a single unsigned compare, so a negative index is out of range rather than
counted from the end (`kernel/buffer.c:113`).

## Errors

| Situation | Raises | Message |
| --- | --- | --- |
| Index out of range (read, write, unset) | `OutOfBoundsException` on PHP ≥ 8.4, `RuntimeException` below | `Index invalid or out of range` |
| Illegal offset **type**, PHP ≥ 8.3 | `TypeError` from `zend_illegal_container_offset()` | engine text, naming the container, e.g. `Cannot access offset of type string on <Ns>\Buffer` |
| Illegal offset type, PHP 8.1 / 8.2 | `TypeError` | `Illegal offset type` |
| Illegal offset type, PHP 8.0 | *(no type check)* | the offset becomes `-1` and is reported by the range check instead |
| `$buffer[] = $v`, PHP ≥ 8.1 | `Error` | `[] operator not supported for <Ns>\Buffer` |
| `$buffer[] = $v`, PHP 8.0 | `RuntimeException` | `Index invalid or out of range` |
| `$buffer[]` in a read context | `Error` | `Cannot use [] for reading` |
| `new Buffer(-1)` | `ValueError` | `<Ns>\Buffer::__construct(): Argument #1 ($size) must be greater than or equal to 0` |
| Invalid `$type` | `ValueError` | `... Argument #2 ($type) must be Buffer::TYPE_DOUBLE or Buffer::TYPE_LONG` |
| Bad `__unserialize()` payload | `Exception` | `Invalid serialization data for <Ns>\Buffer object` |
| `$buffer->anything = 1` | `Error` | engine text for a class with no dynamic properties |
| Writing a value of the "wrong" type | *(nothing)* | there is no type check; the value is cast |

Anchors: out-of-range `kernel/buffer.c:127`, illegal offset `:189`, append `:350`, read-append
`:329`, constructor `:605`/`:610`, unserialize `:793`.

## PHP-version matrix

Three diagnostics moved in php-src, and a `Buffer` follows each of them so that it reports on every
version whatever `SplFixedArray` reports there.

| Behaviour | 8.0 | 8.1 | 8.2 | 8.3 | 8.4 | 8.5 |
| --- | --- | --- | --- | --- | --- | --- |
| Out-of-range class | `RuntimeException` | `RuntimeException` | `RuntimeException` | `RuntimeException` | `OutOfBoundsException` | `OutOfBoundsException` |
| Illegal offset type | no check | `TypeError: Illegal offset type` | same | `TypeError` naming the container | same | same |
| `$buffer[] =` | `RuntimeException` | `Error: [] operator not supported` | same | same | same | same |
| Resource as offset | accepted | accepted, deprecated | same | same | same | same |

`OutOfBoundsException` extends `RuntimeException`, so a `catch (RuntimeException)` written for 8.3
keeps working on 8.4.

### `SplFixedArray` is the test oracle, but not a safe reference for `isset()`

`tests/Extension/BufferTest.php` asserts parity by evaluating the same statement against a live
`SplFixedArray` and comparing transcripts, rewriting the class name in the message
(`tests/Extension/BufferTest.php:621`). That is the right oracle for offset diagnostics and the
reason the table above tracks php-src at all.

It is the wrong oracle for one thing. `SplFixedArray` slots start out `null`, so
`isset($fixed[0])` is **false** on a freshly constructed instance. A numeric buffer's zero is a
value like any other, so `isset($buffer[0])` is **true** (`kernel/buffer.c:386`, asserted separately
at `tests/Extension/BufferTest.php:409`). If you are comparing the two, fill both sides first and
assert the `isset()` difference on its own.

## Behaviour of the standard operations

| Operation | Result |
| --- | --- |
| `count($b)` | element count, via the `count_elements` handler |
| `foreach ($b as $i => $v)` | index ⇒ value, in order, over a materialised copy |
| `clone $b` | deep copy; the element array is duplicated (`kernel/buffer.c:251`) |
| `var_dump($b)` / `print_r($b)` | shows the elements, via `get_debug_info` (`kernel/buffer.c:274`) |
| `json_encode($b)` | a JSON list |
| `serialize($b)` / `unserialize()` | round-trips kind, values and count |
| `$b == $b2` | **always `true`** between any two buffers — no `compare` handler, and no properties for the engine's default comparison to look at. Compare `toArray()` results instead |
| `(array) $b` | `[]` — no `cast_object` handler and no properties. Use `toArray()` |
| `$b[0] += 1`, `$b[0] *= 2` | works — the engine does a read followed by a write |
| `$b[0]++`, `--$b[0]`, `$r =& $b[0]`, a by-reference argument | *Indirect modification of overloaded element* notice, no effect — an element is not an lvalue. See [Usage](./usage.md#traps) |

## See also

- [Usage](./usage.md) — examples and the traps in context.
- [Internals](./internals.md) — the C pointer API and how the handlers are wired.
