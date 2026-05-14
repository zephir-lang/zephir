# Zephir tests using the [sharness framework][:1:]

## Running all the tests

Just use `make` in this directory to run all the tests.
Run with `TEST_VERBOSE=1` to get helpful verbose output.

```shell
$ TEST_VERBOSE=1 make
```

The usual zephir env flags also apply:

```shell
$ ZEPHIR_DEBUG=1 TEST_VERBOSE=1 make
```

Since the tests all output [TAP][:2:] they can be run with any TAP harness.
Here's an example of parallel testing powered by a recent version of [prove][:3:]:

```shell
$ prove --jobs 15 ./t[0-9]*.sh

./t0003-init-errors.sh .. ok
./t0002-generate.sh ..... ok
./t0001-compile.sh ...... ok
All tests successful.
Files=3, Tests=6, 30 wallclock secs ( 0.04 usr  0.01 sys + 33.47 cusr  9.56 csys = 43.08 CPU)
Result: PASS
```

prove and other harnesses come with a lot of useful options.
The `--state` option in particular is very useful:

```shell
# Repeat until no more failures
$ prove -j 15 --state=failed,save ./t[0-9]*.sh
```

## Running just one test

You can run only one test script by launching it like a regular shell script:

```bash
$ ./t0001-compile.sh
```

## Debugging one test

You can use the `-v` option to make it verbose and the `-i` option to make it
stop as soon as one test fails. For example:

```bash
$ ./t0001-compile.sh -v -i
```

[:1:]: https://github.com/mlafeldt/sharness/
[:2:]: https://testanything.org
[:3:]: https://linux.die.net/man/1/prove
