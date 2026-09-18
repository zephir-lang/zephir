
extern zend_class_entry *stub_issue2699_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2699);

PHP_METHOD(Stub_Issue2699, add);
PHP_METHOD(Stub_Issue2699, getComponents);
PHP_METHOD(Stub_Issue2699, getNested);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2699_add, 0, 1, IS_VOID, 0)

	ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2699_getcomponents, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2699_getnested, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2699_method_entry) {
	PHP_ME(Stub_Issue2699, add, arginfo_stub_issue2699_add, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2699, getComponents, arginfo_stub_issue2699_getcomponents, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2699, getNested, arginfo_stub_issue2699_getnested, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
