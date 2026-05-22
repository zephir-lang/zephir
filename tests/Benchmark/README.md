# Zephir Benchmark Suite

Runtime microbenchmarks for the generated PHP extension. Each subject in
`*Bench.php` exercises one Zephir-compiled workload (from `stub/bench.zep`)
paired with a pure-PHP equivalent in the same class, so the headline metric is
the Zephir-over-PHP speedup margin rather than an absolute number that depends
on the runner.

Tracks the work proposed in [#2541](https://github.com/zephir-lang/zephir/issues/2541).

## Running locally

The bench suite needs the `stub` extension built and loadable. Inside any of
the prepared docker containers (`zephir-8.0` ... `zephir-8.5`):

```bash
# Inside the container, /srv is bind-mounted to this repo
cd /srv
php zephir fullclean && php zephir build
php -d extension=ext/modules/stub.so vendor/bin/phpbench run --report=aggregate
```

If you build on the host and run inside the container without fullclean, the
.libs object files from your host's PHP version will conflict. Always
`fullclean` when crossing PHP versions.

## Comparing two branches

CI runs the bench job on every `push` to a non-default branch (see the
`benchmark` job in `.github/workflows/main.yml`). If the pushed branch has an
open PR against `development`, the report is posted as an auto-updating PR
comment; otherwise it is uploaded as an artifact only. The default-branch
push and tag pushes are skipped so the comparison baseline doesn't run
against itself.

Locally, the same flow in two steps:

```bash
# 1. Capture a baseline tagged "base" on the branch you want to compare
#    against. Build the extension first so the bench targets exist.
git checkout development
php zephir fullclean && php zephir build
php -d extension=ext/modules/stub.so vendor/bin/phpbench run \
    --tag=base --progress=none

# 2. Switch to the change you want to evaluate, rebuild, and compare.
git checkout your-branch
php zephir fullclean && php zephir build
php -d extension=ext/modules/stub.so vendor/bin/phpbench run \
    --ref=base --report=aggregate --progress=none
```

The `aggregate` report's `mode` column shows the new throughput and the
percent delta against the tagged baseline.

## Adding a subject

1. Author the workload in `stub/bench.zep`. Keep it focused: one tight
   internal loop, no I/O, no clock reads inside the loop. Re-run
   `php zephir build` so the new method is compiled.
2. Add a method on the relevant `*Bench.php` class (or create a new class if
   the category doesn't fit). Pair it with a `benchPhp*` baseline that does
   the equivalent work in interpreted PHP. Both subjects should fit on screen
   side by side in the `aggregate` report.
3. Decide on `Revs` and `Iterations` annotations. Cheap subjects (sub-μs)
   want `Revs(1000)` so a single PHPBench iteration produces a stable
   measurement. Expensive subjects (hundreds of μs) want `Revs(50)`.

## Interpreting results

The runner the GitHub Actions job uses is a shared `ubuntu-24.04` VM, which
exhibits ±5-20% noise per subject across runs. As a result:

- **Single-digit deltas are inconclusive.** A `-3%` regression on a single
  bench run is not a regression signal; it is noise.
- **Trends across many runs matter more than any one PR comment.** The
  artifact uploaded by the CI job retains the raw report for 14 days; compare
  several runs before drawing conclusions about a specific codegen change.
- **Pure-PHP baselines are the canary.** If a `benchPhp*` baseline moves by
  X%, the matching `benchZephir*` row's delta should be interpreted relative
  to that X, not in absolute terms. The two subjects share the runner, so
  both react to the same noise events.

## When NOT to extend this suite

End-to-end HTTP throughput (Phalcon-style workloads against `wrk`) and kernel
C-level benchmarks (`google/benchmark` over `kernel/*.c`) are out of scope.
PHPBench is for PHP-callable subjects; the other two layers need different
harnesses and belong in their own follow-up issues.
