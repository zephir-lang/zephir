
extern zend_class_entry *stub_issue2824pair_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2824Pair);

PHP_METHOD(Stub_Issue2824Pair, pair);
PHP_METHOD(Stub_Issue2824Pair, spair);
PHP_METHOD(Stub_Issue2824Pair, withDefault);
PHP_METHOD(Stub_Issue2824Pair, withCallable);
PHP_METHOD(Stub_Issue2824Pair, withObject);
PHP_METHOD(Stub_Issue2824Pair, withInt);
PHP_METHOD(Stub_Issue2824Pair, variadic);
PHP_METHOD(Stub_Issue2824Pair, thisPair);
PHP_METHOD(Stub_Issue2824Pair, selfPair);
PHP_METHOD(Stub_Issue2824Pair, staticPair);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2824pair_pair, 0, 2, IS_STRING, 0)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2824pair_spair, 0, 2, IS_STRING, 0)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2824pair_withdefault, 0, 0, IS_STRING, 0)
	ZEND_ARG_INFO_WITH_DEFAULT_VALUE(0, a, "'x'")
	ZEND_ARG_INFO_WITH_DEFAULT_VALUE(0, b, "'y'")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2824pair_withcallable, 0, 0, IS_STRING, 0)
	ZEND_ARG_INFO_WITH_DEFAULT_VALUE(0, c, "null")
	ZEND_ARG_INFO_WITH_DEFAULT_VALUE(0, b, "'y'")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2824pair_withobject, 0, 0, IS_STRING, 0)
	ZEND_ARG_TYPE_MASK(0, o, MAY_BE_OBJECT|MAY_BE_NULL, "null")
	ZEND_ARG_INFO_WITH_DEFAULT_VALUE(0, b, "'y'")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2824pair_withint, 0, 0, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, a, IS_LONG, 0, "5")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, b, IS_LONG, 0, "6")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2824pair_variadic, 0, 1, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_VARIADIC_INFO(0, rest)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2824pair_thispair, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2824pair_selfpair, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2824pair_staticpair, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2824pair_method_entry) {
	PHP_ME(Stub_Issue2824Pair, pair, arginfo_stub_issue2824pair_pair, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2824Pair, spair, arginfo_stub_issue2824pair_spair, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2824Pair, withDefault, arginfo_stub_issue2824pair_withdefault, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2824Pair, withCallable, arginfo_stub_issue2824pair_withcallable, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2824Pair, withObject, arginfo_stub_issue2824pair_withobject, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2824Pair, withInt, arginfo_stub_issue2824pair_withint, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2824Pair, variadic, arginfo_stub_issue2824pair_variadic, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2824Pair, thisPair, arginfo_stub_issue2824pair_thispair, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2824Pair, selfPair, arginfo_stub_issue2824pair_selfpair, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2824Pair, staticPair, arginfo_stub_issue2824pair_staticpair, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
