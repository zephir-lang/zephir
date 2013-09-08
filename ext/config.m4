PHP_ARG_ENABLE(test, whether to enable test, [ --enable-test   Enable Test framework])

if test "$PHP_TEST" = "yes"; then
	AC_DEFINE(HAVE_TEST, 1, [Whether you have Test])
	test_sources="test.c kernel/main.c kernel/memory.c kernel/exception.c kernel/hash.c kernel/debug.c kernel/backtrace.c kernel/object.c kernel/array.c kernel/string.c kernel/fcall.c kernel/alternative/fcall.c kernel/operators.c kernel/concat.c flow.c nativearray.c testinterface.c scallexternal.c scall.c regexdna.c router/route.c router/exception.c mcall.c oo.c fcall.c spectralnorm.c assign.c fibonnaci.c declare.c exceptions.c constantsparent.c arithmetic.c properties/publicproperties.c properties/protectedproperties.c fannkuch.c cast.c oo/oonoconstruct.c oo/ooconstruct.c oo/ooconstructparams.c echoes.c constants.c scallparent.c router.c exception.c"
	PHP_NEW_EXTENSION(test, $test_sources, $ext_shared)
fi
