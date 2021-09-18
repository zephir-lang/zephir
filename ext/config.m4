PHP_ARG_ENABLE(stub, whether to enable stub, [ --enable-stub   Enable Stub])

if test "$PHP_STUB" = "yes"; then

	

	if ! test "x" = "x"; then
		PHP_EVAL_LIBLINE(, STUB_SHARED_LIBADD)
	fi

	AC_DEFINE(HAVE_STUB, 1, [Whether you have Stub])
	stub_sources="stub.c kernel/main.c kernel/memory.c kernel/exception.c kernel/debug.c kernel/backtrace.c kernel/object.c kernel/array.c kernel/string.c kernel/fcall.c kernel/require.c kernel/file.c kernel/operators.c kernel/math.c kernel/concat.c kernel/variables.c kernel/filter.c kernel/iterator.c kernel/time.c kernel/exit.c stub/invokes/abstractprotected.zep.c
	stub/testinterface.zep.c
	stub/oo/extend/exception.zep.c
	stub/issue2165/issueextendinterface.zep.c
	stub/oo/extend/db/exception.zep.c
	stub/scallparent.zep.c
	stub/constantsparent.zep.c
	stub/globals/session/base.zep.c
	stub/interfaces/interfaceint.zep.c
	stub/interfaces/interfaceintsignature.zep.c
	stub/invokes/abstractinvoker.zep.c
	stub/invokes/abstractinvokercomplex.zep.c
	stub/issue2165/issueinterface.zep.c
	stub/methodinterface.zep.c
	stub/oo/abstractstatic.zep.c
	stub/oo/extend/db/query/exception.zep.c
	stub/oo/oodynamica.zep.c
	stub/oo/scopes/hasprivatemethod.zep.c
	stub/oo/scopes/scopetesterinterface.zep.c
	stub/ooimpl/zbeginning.zep.c
	stub/properties/publicproperties.zep.c
	stub/arithmetic.zep.c
	stub/arrayaccessobj.zep.c
	stub/arrayaccesstest.zep.c
	stub/arrayiterator.zep.c
	stub/arrayiteratortest.zep.c
	stub/arraymanipulation.zep.c
	stub/arrayobject.zep.c
	stub/arraysearch.zep.c
	stub/assign.zep.c
	stub/bench/foo.zep.c
	stub/bitwise.zep.c
	stub/branchprediction.zep.c
	stub/builtin/arraymethods.zep.c
	stub/builtin/charmethods.zep.c
	stub/builtin/intmethods.zep.c
	stub/builtin/stringmethods.zep.c
	stub/cast.zep.c
	stub/cblock.zep.c
	stub/chars.zep.c
	stub/closures.zep.c
	stub/compare.zep.c
	stub/concat.zep.c
	stub/constants.zep.c
	stub/constantsinterface.zep.c
	stub/constantsinterfacea.zep.c
	stub/constantsinterfaceb.zep.c
	stub/constructors/issue1803.zep.c
	stub/declaretest.zep.c
	stub/diinterface.zep.c
	stub/echoes.zep.c
	stub/emptytest.zep.c
	stub/evaltest.zep.c
	stub/exception.zep.c
	stub/exceptions.zep.c
	stub/exists.zep.c
	stub/exitdie.zep.c
	stub/extendedinterface.zep.c
	stub/factorial.zep.c
	stub/fannkuch.zep.c
	stub/fasta.zep.c
	stub/fcall.zep.c
	stub/fetchtest.zep.c
	stub/fibonnaci.zep.c
	stub/flow.zep.c
	stub/flow/switchflow.zep.c
	stub/fortytwo.zep.c
	stub/functional.zep.c
	stub/functionexists.zep.c
	stub/functions.zep.c
	stub/geometry.zep.c
	stub/globals.zep.c
	stub/globals/env.zep.c
	stub/globals/post.zep.c
	stub/globals/server.zep.c
	stub/globals/serverrequestfactory.zep.c
	stub/globals/session/child.zep.c
	stub/instance.zep.c
	stub/instanceoff.zep.c
	stub/integration/psr/extendexternal.zep.c
	stub/integration/psr/http/message/messageinterfaceex.zep.c
	stub/interfaces/implementint.zep.c
	stub/interfaces/implementinterface.zep.c
	stub/interfaces/interfacestaticmethod.zep.c
	stub/internalclasses.zep.c
	stub/internalinterfaces.zep.c
	stub/invoke.zep.c
	stub/invokes/invokeprotected.zep.c
	stub/invokes/invokeprotectedcomplex.zep.c
	stub/issettest.zep.c
	stub/issue1134.zep.c
	stub/issue1404.zep.c
	stub/issue1521.zep.c
	stub/issue2165/issue.zep.c
	stub/issue663.zep.c
	stub/issue887.zep.c
	stub/issue893.zep.c
	stub/issue914.zep.c
	stub/issues.zep.c
	stub/json.zep.c
	stub/logical.zep.c
	stub/mcall.zep.c
	stub/mcall/caller.zep.c
	stub/mcallchained.zep.c
	stub/mcalldynamic.zep.c
	stub/mcallinternal.zep.c
	stub/methodabstract.zep.c
	stub/methodargs.zep.c
	stub/namespaces/a/b/sub.zep.c
	stub/namespaces/classentry.zep.c
	stub/nativearray.zep.c
	stub/oo.zep.c
	stub/oo/abstractclass.zep.c
	stub/oo/concretestatic.zep.c
	stub/oo/constantsinterface.zep.c
	stub/oo/deprecatedmethods.zep.c
	stub/oo/dynamicprop.zep.c
	stub/oo/extend/db/query/placeholder/exception.zep.c
	stub/oo/extend/spl/arrayobject.zep.c
	stub/oo/extend/spl/directoryiterator.zep.c
	stub/oo/extend/spl/doublylinkedlist.zep.c
	stub/oo/extend/spl/fileinfo.zep.c
	stub/oo/extend/spl/fileobject.zep.c
	stub/oo/extend/spl/filesystemiterator.zep.c
	stub/oo/extend/spl/fixedarray.zep.c
	stub/oo/extend/spl/globiterator.zep.c
	stub/oo/extend/spl/heap.zep.c
	stub/oo/extend/spl/maxheap.zep.c
	stub/oo/extend/spl/minheap.zep.c
	stub/oo/extend/spl/priorityqueue.zep.c
	stub/oo/extend/spl/queue.zep.c
	stub/oo/extend/spl/recursivedirectoryiterator.zep.c
	stub/oo/extend/spl/stack.zep.c
	stub/oo/extend/spl/tempfileobject.zep.c
	stub/oo/extendpdoclass.zep.c
	stub/oo/ooconstruct.zep.c
	stub/oo/ooconstructparams.zep.c
	stub/oo/oodestruct.zep.c
	stub/oo/oodynamicb.zep.c
	stub/oo/oonativeimplements.zep.c
	stub/oo/oonoconstruct.zep.c
	stub/oo/ooparams.zep.c
	stub/oo/param.zep.c
	stub/oo/propertyaccess.zep.c
	stub/oo/scopes/abstractclass.zep.c
	stub/oo/scopes/abstractclassmagic.zep.c
	stub/oo/scopes/privatescopetester.zep.c
	stub/ooimpl/abeginning.zep.c
	stub/operator.zep.c
	stub/optimizers/isscalar.zep.c
	stub/optimizers/acos.zep.c
	stub/optimizers/arraymerge.zep.c
	stub/optimizers/asin.zep.c
	stub/optimizers/cos.zep.c
	stub/optimizers/createarray.zep.c
	stub/optimizers/ldexp.zep.c
	stub/optimizers/sin.zep.c
	stub/optimizers/sqrt.zep.c
	stub/optimizers/strreplace.zep.c
	stub/optimizers/substr.zep.c
	stub/optimizers/tan.zep.c
	stub/pdostatement.zep.c
	stub/pregmatch.zep.c
	stub/properties/app.zep.c
	stub/properties/extendspublicproperties.zep.c
	stub/properties/getobjectvars.zep.c
	stub/properties/privateproperties.zep.c
	stub/properties/propertyarray.zep.c
	stub/properties/propertyupdate.zep.c
	stub/properties/protectedproperties.zep.c
	stub/properties/staticprivateproperties.zep.c
	stub/properties/staticpropertyarray.zep.c
	stub/properties/staticprotectedproperties.zep.c
	stub/properties/staticpublicproperties.zep.c
	stub/quantum.zep.c
	stub/range.zep.c
	stub/references.zep.c
	stub/reflection.zep.c
	stub/regexdna.zep.c
	stub/requires.zep.c
	stub/requires/external3.zep.c
	stub/resourcetest.zep.c
	stub/returns.zep.c
	stub/router.zep.c
	stub/router/exception.zep.c
	stub/router/route.zep.c
	stub/scall.zep.c
	stub/scalldynamic.zep.c
	stub/scallexternal.zep.c
	stub/scalllateconstruct.zep.c
	stub/scope.zep.c
	stub/sort.zep.c
	stub/spectralnorm.zep.c
	stub/spl.zep.c
	stub/spropertyaccess.zep.c
	stub/statements.zep.c
	stub/strings.zep.c
	stub/stubs.zep.c
	stub/ternary.zep.c
	stub/trytest.zep.c
	stub/typehinting/testabstract.zep.c
	stub/typeinstances.zep.c
	stub/typeoff.zep.c
	stub/types/mixedtype.zep.c
	stub/unknownclass.zep.c
	stub/unsettest.zep.c
	stub/usetest.zep.c
	stub/vars.zep.c
	stub/0__closure.zep.c
	stub/1__closure.zep.c
	stub/2__closure.zep.c
	stub/3__closure.zep.c
	stub/4__closure.zep.c
	stub/5__closure.zep.c
	stub/6__closure.zep.c
	stub/7__closure.zep.c
	stub/8__closure.zep.c
	stub/9__closure.zep.c
	stub/10__closure.zep.c
	stub/11__closure.zep.c
	stub/12__closure.zep.c
	stub/13__closure.zep.c "
	PHP_NEW_EXTENSION(stub, $stub_sources, $ext_shared,, )
	PHP_ADD_BUILD_DIR([$ext_builddir/kernel/])
	for dir in "stub stub/bench stub/builtin stub/constructors stub/flow stub/globals stub/globals/session stub/integration/psr stub/integration/psr/http/message stub/interfaces stub/invokes stub/issue2165 stub/mcall stub/namespaces stub/namespaces/a/b stub/oo stub/oo/extend stub/oo/extend/db stub/oo/extend/db/query stub/oo/extend/db/query/placeholder stub/oo/extend/spl stub/oo/scopes stub/ooimpl stub/optimizers stub/properties stub/requires stub/router stub/typehinting stub/types"; do
		PHP_ADD_BUILD_DIR([$ext_builddir/$dir])
	done
	PHP_SUBST(STUB_SHARED_LIBADD)

	old_CPPFLAGS=$CPPFLAGS
	CPPFLAGS="$CPPFLAGS $INCLUDES"

	AC_CHECK_DECL(
		[HAVE_BUNDLED_PCRE],
		[
			AC_CHECK_HEADERS(
				[ext/pcre/php_pcre.h],
				[
					PHP_ADD_EXTENSION_DEP([stub], [pcre])
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
					PHP_ADD_EXTENSION_DEP([stub], [json])
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

	PHP_INSTALL_HEADERS([ext/stub], [php_STUB.h])

fi
