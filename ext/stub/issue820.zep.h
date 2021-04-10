
extern zend_class_entry *stub_issue820_ce;

ZEPHIR_INIT_CLASS(Stub_Issue820);

PHP_METHOD(Stub_Issue820, iterate);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue820_iterate, 0, 0, 1)
	ZEND_ARG_INFO(0, obj)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue820_method_entry) {
	PHP_ME(Stub_Issue820, iterate, arginfo_stub_issue820_iterate, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
