
extern zend_class_entry *stub_issue2824_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2824);

PHP_METHOD(Stub_Issue2824, typed);
PHP_METHOD(Stub_Issue2824, untyped);
PHP_METHOD(Stub_Issue2824, positionalThenNamed);
PHP_METHOD(Stub_Issue2824, typedMixed);
PHP_METHOD(Stub_Issue2824, skipped);
PHP_METHOD(Stub_Issue2824, skippedUntyped);
PHP_METHOD(Stub_Issue2824, skippedCallable);
PHP_METHOD(Stub_Issue2824, skippedObject);
PHP_METHOD(Stub_Issue2824, skippedInt);
PHP_METHOD(Stub_Issue2824, variadicExtras);
PHP_METHOD(Stub_Issue2824, unknownName);
PHP_METHOD(Stub_Issue2824, overwrite);
PHP_METHOD(Stub_Issue2824, statement);
PHP_METHOD(Stub_Issue2824, classStatic);
PHP_METHOD(Stub_Issue2824, dynamicClass);
PHP_METHOD(Stub_Issue2824, dynamicMethod);
PHP_METHOD(Stub_Issue2824, userFunction);
PHP_METHOD(Stub_Issue2824, numberFormat);
PHP_METHOD(Stub_Issue2824, strposNamed);
PHP_METHOD(Stub_Issue2824, inArrayNamed);
PHP_METHOD(Stub_Issue2824, pregMatchNamed);
PHP_METHOD(Stub_Issue2824, positionalByReference);
PHP_METHOD(Stub_Issue2824, namedArgumentReleased);
PHP_METHOD(Stub_Issue2824, byReferenceLeftUnassigned);
PHP_METHOD(Stub_Issue2824, construct);
PHP_METHOD(Stub_Issue2824, closure);
PHP_METHOD(Stub_Issue2824, callUserFunc);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2824_typed, 0, 1, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0, p, Stub\\Issue2824Pair, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2824_untyped, 0, 1, IS_STRING, 0)
	ZEND_ARG_INFO(0, p)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2824_positionalthennamed, 0, 1, IS_STRING, 0)
	ZEND_ARG_INFO(0, p)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2824_typedmixed, 0, 1, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0, p, Stub\\Issue2824Pair, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2824_skipped, 0, 1, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0, p, Stub\\Issue2824Pair, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2824_skippeduntyped, 0, 1, IS_STRING, 0)
	ZEND_ARG_INFO(0, p)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2824_skippedcallable, 0, 1, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0, p, Stub\\Issue2824Pair, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2824_skippedobject, 0, 1, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0, p, Stub\\Issue2824Pair, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2824_skippedint, 0, 1, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0, p, Stub\\Issue2824Pair, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2824_variadicextras, 0, 1, IS_ARRAY, 0)
	ZEND_ARG_OBJ_INFO(0, p, Stub\\Issue2824Pair, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2824_unknownname, 0, 1, IS_STRING, 0)
	ZEND_ARG_INFO(0, p)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2824_overwrite, 0, 1, IS_STRING, 0)
	ZEND_ARG_INFO(0, p)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2824_statement, 0, 1, IS_VOID, 0)

	ZEND_ARG_OBJ_INFO(0, p, Stub\\Issue2824Pair, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2824_classstatic, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2824_dynamicclass, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, className, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2824_dynamicmethod, 0, 2, IS_STRING, 0)
	ZEND_ARG_INFO(0, p)
	ZEND_ARG_TYPE_INFO(0, method, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2824_userfunction, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2824_numberformat, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2824_strposnamed, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2824_inarraynamed, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2824_pregmatchnamed, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2824_positionalbyreference, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2824_namedargumentreleased, 0, 1, IS_ARRAY, 0)
	ZEND_ARG_OBJ_INFO(0, p, Stub\\Issue2824Pair, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2824_byreferenceleftunassigned, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2824_construct, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2824_closure, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2824_calluserfunc, 0, 1, IS_STRING, 0)
	ZEND_ARG_INFO(0, p)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2824_method_entry) {
	PHP_ME(Stub_Issue2824, typed, arginfo_stub_issue2824_typed, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2824, untyped, arginfo_stub_issue2824_untyped, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2824, positionalThenNamed, arginfo_stub_issue2824_positionalthennamed, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2824, typedMixed, arginfo_stub_issue2824_typedmixed, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2824, skipped, arginfo_stub_issue2824_skipped, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2824, skippedUntyped, arginfo_stub_issue2824_skippeduntyped, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2824, skippedCallable, arginfo_stub_issue2824_skippedcallable, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2824, skippedObject, arginfo_stub_issue2824_skippedobject, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2824, skippedInt, arginfo_stub_issue2824_skippedint, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2824, variadicExtras, arginfo_stub_issue2824_variadicextras, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2824, unknownName, arginfo_stub_issue2824_unknownname, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2824, overwrite, arginfo_stub_issue2824_overwrite, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2824, statement, arginfo_stub_issue2824_statement, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2824, classStatic, arginfo_stub_issue2824_classstatic, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2824, dynamicClass, arginfo_stub_issue2824_dynamicclass, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2824, dynamicMethod, arginfo_stub_issue2824_dynamicmethod, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2824, userFunction, arginfo_stub_issue2824_userfunction, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2824, numberFormat, arginfo_stub_issue2824_numberformat, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Issue2824, strposNamed, arginfo_stub_issue2824_strposnamed, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2824, inArrayNamed, arginfo_stub_issue2824_inarraynamed, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2824, pregMatchNamed, arginfo_stub_issue2824_pregmatchnamed, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2824, positionalByReference, arginfo_stub_issue2824_positionalbyreference, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2824, namedArgumentReleased, arginfo_stub_issue2824_namedargumentreleased, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2824, byReferenceLeftUnassigned, arginfo_stub_issue2824_byreferenceleftunassigned, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2824, construct, arginfo_stub_issue2824_construct, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2824, closure, arginfo_stub_issue2824_closure, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2824, callUserFunc, arginfo_stub_issue2824_calluserfunc, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
