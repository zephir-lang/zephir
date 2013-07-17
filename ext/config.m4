PHP_ARG_ENABLE(test, whether to enable test, [ --enable-test   Enable test framework])

if test "$PHP_TEST" = "yes"; then
  AC_DEFINE(HAVE_TEST, 1, [Whether you have Test])
  phalcon_sources="test.c router.c"

  PHP_NEW_EXTENSION(test, $test_sources, $ext_shared)
fi
