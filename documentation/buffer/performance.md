# `<Ns>\Buffer` — performance

## Where the win comes from

Two independent mechanisms. They compound, but it is worth keeping them apart when reasoning about
whether a given workload will benefit.

**Storage.** A PHP array of `n` floats costs a zval plus a hash-table slot per element. A buffer is
one `ecalloc(n, sizeof(double))` (`kernel/buffer.c:85`) — roughly half the bytes for `double`
elements, and contiguous, so it is cache- and prefetcher-friendly, and it can be handed to a C
numeric library as a pointer with no packing step at all.

**Access.** `buf[i]` in Zephir source compiles to a `zephir_array_*` kernel call. Without the fast
path that call would see an ordinary `ArrayAccess` object and dispatch a full `offsetGet()` or
`offsetSet()` **method call** per element. The fast path (`kernel/array.c`, six sites) puts a
class-entry pointer compare ahead of the `ArrayAccess` branch and, on a match, does a direct scalar
load or store without leaving C.

The second mechanism is why `buf[i]` from Zephir is worth using at all. Without the fast path a
Zephir-side `buf[i]` read was measurably *slower* than the same read written in plain PHP — PHP goes
through the `read_dimension` object handler, while Zephir's kernel was dispatching a full method
call. With it, a compiled `buf[i]` read beats a compiled PHP-array read (measured below).

### No type inference is involved

This is worth stating because it is the usual first question. The compiler cannot prove that
`this->buf` holds a `Buffer`: every property read is typed `undefined`
(`src/Expression/PropertyAccess.php:279`), and `Buffer` is a hand-written kernel class with no
`.zep` definition to hint against anyway.

It does not matter. The dispatch is a runtime check — `Z_OBJCE_P(arr) == zephir_buffer_ce`
(`kernel/buffer.h:76`) — one predictable compare, placed where the kernel was already going to test
the container's type. Declaring the parameter `var` costs nothing.

## The boundary cost

`fromArray()` and `toArray()` are both O(n) conversions, and they are not cheap in absolute terms.
That produces the single most important rule for using the class:

> The payoff scales with the length of the chain between conversions, not with the speed of any one
> operation.

Convert in, do one operation, convert out, and you will usually lose. Convert in, do five
operations, convert out, and the conversions amortise away.

## Measuring it in this repository

`tests/Benchmark/BufferBench.php` drives the Zephir-side workloads in `stub/bench.zep:551`. Subjects:

| Subject | Measures |
| --- | --- |
| `benchZephirBufferSum` | compiled `buf[i]` read via the kernel fast path |
| `benchZephirArraySum` | compiled read over a PHP array (control) |
| `benchPhpArraySum` | interpreted PHP array read (baseline) |
| `benchPhpBufferSum` | interpreted `$buffer[$i]` — the `read_dimension` handler, no fast path |
| `benchZephirBufferWrite` | compiled `let buf[i] = v` |
| `benchBufferToArray` | the materialisation cost the buffer exists to avoid |
| `benchBufferFromArray` | the ingest cost |

Run them inside one of the prepared containers (`zephir-8.0` … `zephir-8.5`), where `/srv` is
bind-mounted to the repository:

```bash
cd /srv
php zephir fullclean && php zephir build
php -d extension=ext/modules/stub.so vendor/bin/phpbench run --report=aggregate
```

The two comparisons that answer the two questions above:

- **Is the fast path working?** `benchZephirBufferSum` against `benchZephirArraySum`. Both are the
  same compiled loop; only the container differs. If the buffer subject is not at least as fast as
  the array one, the fast path is not being taken — check that `ZEPHIR_BUFFER_ENABLED` is actually
  defined in the generated header.
- **Is a buffer worth it for my data?** Take that difference and weigh it against
  `benchBufferFromArray` + `benchBufferToArray` for your `n` and your number of operations per
  round trip.

`benchPhpBufferSum` is the userland control rather than a fast-path measurement: from PHP,
`$buffer[$i]` goes through the `read_dimension` object handler, not through `offsetGet()`, and the
kernel fast path is not involved at all.

For an A/B across two builds, use the `--tag=base` / `--ref=base` flow already documented in
[`tests/Benchmark/README.md`](../../tests/Benchmark/README.md). Note that the reported percentage is
**time**, so a negative delta means faster.

### Observed numbers

One run on the development host, PHP 8.3.31, `n = 1000` per subject. Reproduce it with the command
above rather than quoting these — they depend on the CPU, the PHP build and the element count.

| Subject | Throughput | Per 1000-element loop |
| --- | --- | --- |
| `benchZephirBufferWrite` | 0.219 ops/μs | 4.6 μs |
| `benchZephirBufferSum` | 0.147 ops/μs | 6.8 μs |
| `benchZephirArraySum` | 0.125 ops/μs | 8.0 μs |
| `benchPhpArraySum` | 0.032 ops/μs | 31.7 μs |
| `benchPhpBufferSum` | 0.026 ops/μs | 37.8 μs |
| `benchBufferFromArray` | 0.680 ops/μs | 1.5 μs |
| `benchBufferToArray` | 0.155 ops/μs | 6.5 μs |

Three things to read out of it:

- **A compiled `buf[i]` read beats a compiled PHP-array read** — 6.8 μs against 8.0 μs. That is the
  fast path doing its job; a buffer element costs less than a hash lookup.
- **Doing the loop in Zephir is worth far more than the container choice** — 6.8 μs against 37.8 μs
  for the same loop written in PHP over the same buffer, about 5.6×. From PHP, indexing a buffer is
  *slower* than indexing an array (37.8 vs 31.7 μs), because PHP's array access is heavily optimised
  and the buffer goes through an object handler. A buffer pays off in compiled code and at the C
  boundary, not in userland loops.
- **The boundary costs are the size of an operation, not a rounding error** — `toArray()` at 6.5 μs
  costs about the same as one entire 1000-element read loop, and `fromArray()` at 1.5 μs about a
  fifth of one. Which is the arithmetic behind the rule at the top of this section.

**Not reproducible from this tree:** during development, the fast path was measured against a build
with it disabled — where each `buf[i]` becomes a real `offsetGet()` / `offsetSet()` method call — at
roughly **27× for reads and 48× for writes**, and a Zephir-side `buf[i]` read was about **5× slower
than the same read written in PHP** before the fast path existed. Reproducing those requires
reverting the `kernel/array.c` guards, so treat them as background on why the fast path exists
rather than as numbers you can check.

A whole-suite sweep with the buffer compiled out versus compiled in showed every other subject
inside noise, which is the expected result: every fast-path site is behind `#ifdef` and, when
present, behind an `UNEXPECTED()` pointer compare.

## Case study: Tensor

**External, and not reproducible from this repository.** These numbers come from a prototype
`Tensor\BufferVector` built against [Tensor](https://github.com/RubixML/Tensor) during the design of
this feature, compared with the existing `Tensor\Vector` compiled into the **same `.so`**, so the
comparison is apples to apples. One prototype, one host, one workload — it illustrates the shape of
the win, it is not a promise about yours.

At `n = 1e6`:

| Operation | PHP-array path | Buffer path |
| --- | --- | --- |
| `add` | 32.9 ms | 3.9 ms |
| `multiply` | 35.6 ms | 3.7 ms |
| `dot` | 16.8 ms | 0.40 ms |
| chain `a.add(b).multiply(c)` | 69.5 ms | 8.3 ms |

`dot` is the outlier at ~42× because the array path never reached `cblas_ddot` at all — the packing
step dominated it so thoroughly that the BLAS call was never worth making. That is the general
lesson: a contiguous buffer does not just make the existing path faster, it makes a different path
viable.

At `n = 1e5`, where the data fits in cache, the ratios rose to 50–70×.

Storage for 1e6 doubles fell from **16.8 MB to 8.0 MB**.

And the boundary costs, which are what cap the win on a *single* operation:

| Boundary | Cost at n = 1e6 |
| --- | --- |
| `build` (array → buffer) | 3.9 ms |
| `asArray` (buffer → array) | 11.2 ms |

Put those next to the per-operation figures and the rule falls out: one `add` costs 3.9 ms against a
15.1 ms round trip, so a single operation loses. Four operations win comfortably.

## Measuring on your own host

One trap is worth carrying here because it silently poisons results rather than failing loudly.

On a CPU without AVX — a QEMU vCPU advertising only up to `sse4_2`, for instance — OpenBLAS may
dispatch a kernel built for instructions the CPU does not have, and the process dies with `SIGILL`
(exit status 132). This happens on pristine sources too, so it is easy to misread as a regression in
whatever you are testing. Pin the kernel:

```bash
export OPENBLAS_CORETYPE=NEHALEM
```

And do not hide it behind a pipe. `phpbench ... | tail` exits 0 even when the left-hand side died,
so the failure disappears:

```bash
php -d extension=ext/modules/stub.so vendor/bin/phpbench run --report=aggregate | tail -20
echo "${PIPESTATUS[0]}"    # this is the exit status that matters
```

## See also

- [Usage](./usage.md) — including why `foreach` is not a fast path.
- [Internals](./internals.md) — the six fast-path sites and the raw-pointer C API.
