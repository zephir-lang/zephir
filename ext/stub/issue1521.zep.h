
extern zend_class_entry *stub_issue1521_ce;

ZEPHIR_INIT_CLASS(Stub_Issue1521);

PHP_METHOD(Stub_Issue1521, test);

ZEPHIR_INIT_FUNCS(stub_issue1521_method_entry) {
	PHP_ME(Stub_Issue1521, test, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
