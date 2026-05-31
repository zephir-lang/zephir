
extern zend_class_entry *stub_issue2063_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2063);

PHP_METHOD(Stub_Issue2063, add);
PHP_METHOD(Stub_Issue2063, add2);
PHP_METHOD(Stub_Issue2063, addLiteral);
PHP_METHOD(Stub_Issue2063, get);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2063_add, 0, 1, IS_VOID, 0)

	ZEND_ARG_TYPE_INFO(0, value, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2063_add2, 0, 1, IS_VOID, 0)

	ZEND_ARG_TYPE_INFO(0, value, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2063_addliteral, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2063_get, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2063_method_entry) {
	PHP_ME(Stub_Issue2063, add, arginfo_stub_issue2063_add, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2063, add2, arginfo_stub_issue2063_add2, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2063, addLiteral, arginfo_stub_issue2063_addliteral, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2063, get, arginfo_stub_issue2063_get, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
