
extern zend_class_entry *stub_cblock_ce;

ZEPHIR_INIT_CLASS(Stub_Cblock);

PHP_METHOD(Stub_Cblock, testCblock1);
PHP_METHOD(Stub_Cblock, testCblock2);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_cblock_testcblock1, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_cblock_testcblock2, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_cblock_method_entry) {
	PHP_ME(Stub_Cblock, testCblock1, arginfo_stub_cblock_testcblock1, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cblock, testCblock2, arginfo_stub_cblock_testcblock2, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
