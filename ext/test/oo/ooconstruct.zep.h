
extern zend_class_entry *test_oo_ooconstruct_ce;

ZEPHIR_INIT_CLASS(Test_Oo_OoConstruct);

PHP_METHOD(Test_Oo_OoConstruct, __construct);

ZEPHIR_INIT_FUNCS(test_oo_ooconstruct_method_entry) {
	PHP_ME(Test_Oo_OoConstruct, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_FE_END
};
