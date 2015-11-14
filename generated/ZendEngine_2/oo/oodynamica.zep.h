
extern zend_class_entry *test_oo_oodynamica_ce;

ZEPHIR_INIT_CLASS(Test_Oo_OoDynamicA);

PHP_METHOD(Test_Oo_OoDynamicA, getNew);

ZEPHIR_INIT_FUNCS(test_oo_oodynamica_method_entry) {
	PHP_ME(Test_Oo_OoDynamicA, getNew, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
