PHP_ARG_ENABLE(%PROJECT_LOWER%, whether to enable %PROJECT_LOWER%, [ --enable-%PROJECT_LOWER%   Enable %PROJECT_CAMELIZE%])

if test "$PHP_%PROJECT_UPPER%" = "yes"; then

	%PROJECT_PACKAGE_DEPENDENCIES%

	if ! test "x%PROJECT_EXTRA_LIBS%" = "x"; then
		PHP_EVAL_LIBLINE(%PROJECT_EXTRA_LIBS%, %PROJECT_UPPER%_SHARED_LIBADD)
	fi

	AC_DEFINE(HAVE_%PROJECT_UPPER%, 1, [Whether you have %PROJECT_CAMELIZE%])
	%PROJECT_LOWER%_sources="%PROJECT_LOWER_SAFE%.c kernel/main.c kernel/memory.c kernel/exception.c kernel/hash.c kernel/debug.c kernel/backtrace.c kernel/object.c kernel/array.c kernel/extended/array.c kernel/string.c kernel/fcall.c kernel/extended/fcall.c kernel/require.c kernel/file.c kernel/operators.c kernel/math.c kernel/concat.c kernel/variables.c kernel/filter.c kernel/iterator.c kernel/time.c kernel/exit.c %FILES_COMPILED% %EXTRA_FILES_COMPILED%"
	PHP_NEW_EXTENSION(%PROJECT_LOWER%, $%PROJECT_LOWER%_sources, $ext_shared,, %PROJECT_EXTRA_CFLAGS%)
	PHP_SUBST(%PROJECT_UPPER%_SHARED_LIBADD)

	old_CPPFLAGS=$CPPFLAGS
	CPPFLAGS="$CPPFLAGS $INCLUDES"

	AC_CHECK_DECL(
		[HAVE_BUNDLED_PCRE],
		[
			AC_CHECK_HEADERS(
				[ext/pcre/php_pcre.h],
				[
					PHP_ADD_EXTENSION_DEP([%PROJECT_LOWER%], [pcre])
					AC_DEFINE([ZEPHIR_USE_PHP_PCRE], [1], [Whether PHP pcre extension is present at compile time])
				],
				,
				[[#include "main/php.h"]]
			)
		],
		,
		[[#include "php_config.h"]]
	)

	AC_CHECK_DECL(
		[HAVE_JSON],
		[
			AC_CHECK_HEADERS(
				[ext/json/php_json.h],
				[
					PHP_ADD_EXTENSION_DEP([%PROJECT_LOWER%], [json])
					AC_DEFINE([ZEPHIR_USE_PHP_JSON], [1], [Whether PHP json extension is present at compile time])
				],
				,
				[[#include "main/php.h"]]
			)
		],
		,
		[[#include "php_config.h"]]
	)

	CPPFLAGS=$old_CPPFLAGS

	PHP_INSTALL_HEADERS([ext/%PROJECT_LOWER%], [%HEADERS_COMPILED%])

fi
