
extern zend_class_entry *stub_issue1521_ce;

ZEPHIR_INIT_CLASS(Stub_Issue1521);

PHP_METHOD(Stub_Issue1521, test);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue1521_test, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue1521_method_entry) {
PHP_ME(Stub_Issue1521, test, arginfo_stub_issue1521_test, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
