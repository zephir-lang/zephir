
extern zend_class_entry *stub_range_ce;

ZEPHIR_INIT_CLASS(Stub_Range);

PHP_METHOD(Stub_Range, inclusive1);
PHP_METHOD(Stub_Range, exclusive1);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_range_inclusive1, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_range_exclusive1, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_range_method_entry) {
	PHP_ME(Stub_Range, inclusive1, arginfo_stub_range_inclusive1, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Range, exclusive1, arginfo_stub_range_exclusive1, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
