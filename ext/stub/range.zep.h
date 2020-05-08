
extern zend_class_entry *stub_range_ce;

ZEPHIR_INIT_CLASS(Stub_Range);

PHP_METHOD(Stub_Range, inclusive1);
PHP_METHOD(Stub_Range, exclusive1);

ZEPHIR_INIT_FUNCS(stub_range_method_entry) {
	PHP_ME(Stub_Range, inclusive1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Range, exclusive1, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
