PHP_ARG_ENABLE(test, whether to enable test, [ --enable-test   Enable Test framework])

if test "$PHP_TEST" = "yes"; then
	AC_DEFINE(HAVE_TEST, 1, [Whether you have Test])
	test_sources="test.c kernel/main.c kernel/memory.c kernel/exception.c kernel/hash.c kernel/debug.c kernel/backtrace.c kernel/object.c kernel/array.c kernel/string.c kernel/fcall.c kernel/alternative/fcall.c kernel/operators.c kernel/concat.c arithmetic.c assign.c cast.c constants.c constantsparent.c declare.c echoes.c exception.c exceptions.c fannkuch.c fcall.c fibonnaci.c flow.c mcall.c nativearray.c oo/ooconstruct.c oo/ooconstructparams.c oo/oonoconstruct.c oo.c properties/protectedproperties.c properties/publicproperties.c regexdna.c router/exception.c router/route.c router.c scall.c scallparent.c spectralnorm.c testinterface.c"
	PHP_NEW_EXTENSION(test, $test_sources, $ext_shared)
fi
