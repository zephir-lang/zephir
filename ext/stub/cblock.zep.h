
extern zend_class_entry *stub_cblock_ce;

ZEPHIR_INIT_CLASS(Stub_Cblock);

PHP_METHOD(Stub_Cblock, testCblock1);
PHP_METHOD(Stub_Cblock, testCblock2);

ZEPHIR_INIT_FUNCS(stub_cblock_method_entry) {
	PHP_ME(Stub_Cblock, testCblock1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cblock, testCblock2, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
