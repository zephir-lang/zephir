
extern zend_class_entry *stub_variadic_ce;

ZEPHIR_INIT_CLASS(Stub_Variadic);

PHP_METHOD(Stub_Variadic, collect);
PHP_METHOD(Stub_Variadic, sum);
PHP_METHOD(Stub_Variadic, countExtra);
PHP_METHOD(Stub_Variadic, joinStrings);
PHP_METHOD(Stub_Variadic, describe);
PHP_METHOD(Stub_Variadic, staticCollect);
PHP_METHOD(Stub_Variadic, totalLength);
PHP_METHOD(Stub_Variadic, classNames);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_variadic_collect, 0, 0, IS_ARRAY, 0)
	ZEND_ARG_VARIADIC_INFO(0, args)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_variadic_sum, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, first, IS_LONG, 0)
	ZEND_ARG_VARIADIC_INFO(0, rest)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_variadic_countextra, 0, 1, IS_LONG, 0)
	ZEND_ARG_INFO(0, required)
	ZEND_ARG_VARIADIC_INFO(0, rest)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_variadic_joinstrings, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, glue, IS_STRING, 0)
	ZEND_ARG_VARIADIC_TYPE_INFO(0, parts, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_variadic_describe, 0, 1, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, req, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, opt, IS_LONG, 0, "0")
	ZEND_ARG_VARIADIC_INFO(0, params)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_variadic_staticcollect, 0, 0, IS_ARRAY, 0)
	ZEND_ARG_VARIADIC_INFO(0, args)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_variadic_totallength, 0, 0, IS_LONG, 0)
	ZEND_ARG_VARIADIC_TYPE_INFO(0, rows, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_variadic_classnames, 0, 0, IS_ARRAY, 0)
	ZEND_ARG_VARIADIC_INFO(0, objects)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_variadic_method_entry) {
	PHP_ME(Stub_Variadic, collect, arginfo_stub_variadic_collect, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Variadic, sum, arginfo_stub_variadic_sum, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Variadic, countExtra, arginfo_stub_variadic_countextra, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Variadic, joinStrings, arginfo_stub_variadic_joinstrings, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Variadic, describe, arginfo_stub_variadic_describe, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Variadic, staticCollect, arginfo_stub_variadic_staticcollect, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Variadic, totalLength, arginfo_stub_variadic_totallength, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Variadic, classNames, arginfo_stub_variadic_classnames, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
