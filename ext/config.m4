PHP_ARG_ENABLE(test, whether to enable test, [ --enable-test   Enable Test framework])

if test "$PHP_TEST" = "yes"; then
	AC_DEFINE(HAVE_TEST, 1, [Whether you have Test])
	test_sources="test.c kernel/main.c kernel/memory.c kernel/exception.c kernel/hash.c kernel/debug.c kernel/backtrace.c kernel/object.c kernel/array.c kernel/string.c kernel/fcall.c kernel/alternative/fcall.c kernel/operators.c flow.c arithmetic.c oo.c router/route.c router/exception.c assign.c oo/ooconstructparams.c oo/ooconstruct.c oo/oonoconstruct.c nativearray.c router.c fibonnaci.c"
	PHP_NEW_EXTENSION(test, $test_sources, $ext_shared)
fi
