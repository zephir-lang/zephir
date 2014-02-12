PHP_ARG_ENABLE(test, whether to enable test, [ --enable-test   Enable Test])

if test "$PHP_TEST" = "yes"; then
	AC_DEFINE(HAVE_TEST, 1, [Whether you have Test])
	test_sources="test.c kernel/main.c kernel/memory.c kernel/exception.c kernel/hash.c kernel/debug.c kernel/backtrace.c kernel/object.c kernel/array.c kernel/extended/array.c kernel/string.c kernel/fcall.c kernel/require.c kernel/alternative/fcall.c kernel/file.c kernel/operators.c kernel/concat.c kernel/variables.c test/arithmetic.c test/assign.c test/bitwise.c test/branchprediction.c test/builtinmethods.c test/cast.c test/cblock.c test/concat.c test/constants.c test/constantsinterface.c test/constantsinterfacea.c test/constantsinterfaceb.c test/constantsparent.c test/declare.c test/echoes.c test/emptytest.c test/exception.c test/exceptions.c test/exists.c test/factorial.c test/fannkuch.c test/fcall.c test/fetchtest.c test/fibonnaci.c test/flow.c test/fortytwo.c test/globals.c test/instanceoff.c test/internalinterfaces.c test/issettest.c test/json.c test/logical.c test/mcall.c test/mcallchained.c test/nativearray.c test/oo.c test/oo/constantsinterface.c test/oo/ooconstruct.c test/oo/ooconstructparams.c test/oo/oodynamica.c test/oo/oodynamicb.c test/oo/oonativeimplements.c test/oo/oonoconstruct.c test/oo/ooparams.c test/oo/propertyaccess.c test/pregmatch.c test/properties/privateproperties.c test/properties/protectedproperties.c test/properties/publicproperties.c test/properties/staticprotectedproperties.c test/properties/staticpublicproperties.c test/regexdna.c test/requires.c test/returns.c test/router.c test/router/exception.c test/router/route.c test/scall.c test/scallexternal.c test/scallparent.c test/spectralnorm.c test/spropertyaccess.c test/strings.c test/ternary.c test/testinterface.c test/trie.c test/typeoff.c test/unsettest.c test/vars.c "
	PHP_NEW_EXTENSION(test, $test_sources, $ext_shared)

	old_CPPFLAGS=$CPPFLAGS
	CPPFLAGS="$CPPFLAGS $INCLUDES"

	AC_CHECK_DECL(
		[HAVE_BUNDLED_PCRE],
		[
			AC_CHECK_HEADERS(
				[ext/pcre/php_pcre.h],
				[
					PHP_ADD_EXTENSION_DEP([test], [pcre])
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
					PHP_ADD_EXTENSION_DEP([test], [json])
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
fi
