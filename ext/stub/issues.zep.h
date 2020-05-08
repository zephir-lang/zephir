
extern zend_class_entry *stub_issues_ce;

ZEPHIR_INIT_CLASS(Stub_Issues);

PHP_METHOD(Stub_Issues, setAdapter);
PHP_METHOD(Stub_Issues, someMethod);
PHP_METHOD(Stub_Issues, test);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issues_setadapter, 0, 0, 1)
	ZEND_ARG_INFO(0, adapter)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issues_somemethod, 0, 0, 1)
	ZEND_ARG_INFO(0, methodName)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issues_method_entry) {
	PHP_ME(Stub_Issues, setAdapter, arginfo_stub_issues_setadapter, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issues, someMethod, arginfo_stub_issues_somemethod, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issues, test, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
