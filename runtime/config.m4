dnl
dnl $Id$
dnl

PHP_ARG_ENABLE(zephir, whether to enable zephir, [ --enable-zephir   Enable Zephir])

if test "$PHP_ZEPHIR" = "yes"; then

	AC_DEFINE(HAVE_ZEPHIR, 1, [Whether you have Zephir])
	zephir_sources="zephir.c kernel/main.c kernel/memory.c kernel/fcall.c kernel/exceptions.c kernel/operators.c kernel/string.c parser.c scanner.c builder.c utils.c classes.c blocks.c expr.c symtable.c variable.c errors.c fcall.c statements/echo.c statements/loop.c statements/let.c statements/if.c statements/while.c statements/declare.c statements/return.c statements/break.c operators/arithmetical.c operators/comparison.c optimizers/evalexpr.c"

	dnl Link LLVM libraries:
	LLVM_LDFLAGS=`llvm-config-3.3 --libs --ldflags core analysis executionengine jit interpreter native`
	LLVM_CFLAGS=`llvm-config-3.3 --cflags`
	LDFLAGS="$LDFLAGS -Wl,-rpath $LLVM_LDFLAGS"
	CFLAGS="$CFLAGS -Wl,-rpath $LLVM_CFLAGS -O0 -g3 -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS"

	dnl Check for stdc++:
	LIBNAME=stdc++
	AC_MSG_CHECKING([for stdc++])
	AC_LANG_SAVE
	AC_LANG_CPLUSPLUS
	AC_TRY_COMPILE(
	[
		#include <string>
		using namespace std;
	],[
		string dummy;
	],[
		AC_MSG_RESULT(yes)
		PHP_ADD_LIBRARY($LIBNAME, , ZEPHIR_SHARED_LIBADD)
	],[
		AC_MSG_ERROR([wrong stdc++ library not found])
	])
	AC_LANG_RESTORE

	PHP_SUBST(ZEPHIR_SHARED_LIBADD)

	PHP_REQUIRE_CXX()
	PHP_NEW_EXTENSION(zephir, $zephir_sources, $ext_shared,,, 1)

	PHP_ADD_MAKEFILE_FRAGMENT
fi
