# Zephir tests using the [sharness framework][:1:]

## Running all the tests

Just use `make` in this directory to run all the tests.
Run with `TEST_VERBOSE=1` to get helpful verbose output.

```bash
$ TEST_VERBOSE=1 make
```

The usual zephir env flags also apply:

```bash
$ ZEPHIR_DEBUG=1 TEST_VERBOSE=1 make
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
