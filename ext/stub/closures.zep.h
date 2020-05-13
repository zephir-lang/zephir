
extern zend_class_entry *stub_closures_ce;

ZEPHIR_INIT_CLASS(Stub_Closures);

PHP_METHOD(Stub_Closures, simple1);
PHP_METHOD(Stub_Closures, simple2);
PHP_METHOD(Stub_Closures, simple3);
PHP_METHOD(Stub_Closures, simple4);
PHP_METHOD(Stub_Closures, simple5);
PHP_METHOD(Stub_Closures, arrow1);
PHP_METHOD(Stub_Closures, arrow2);
PHP_METHOD(Stub_Closures, testUseCommand);
PHP_METHOD(Stub_Closures, issue1860);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_closures_issue1860, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, abc, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_closures_method_entry) {
	PHP_ME(Stub_Closures, simple1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Closures, simple2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Closures, simple3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Closures, simple4, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Closures, simple5, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Closures, arrow1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Closures, arrow2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Closures, testUseCommand, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Closures, issue1860, arginfo_stub_closures_issue1860, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
