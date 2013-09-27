
extern zend_class_entry *test_fibonnaci_ce;

ZEPHIR_INIT_CLASS(Test_Fibonnaci);

PHP_METHOD(Test_Fibonnaci, fibInt);
PHP_METHOD(Test_Fibonnaci, fibDouble);
PHP_METHOD(Test_Fibonnaci, fibArray);
PHP_METHOD(Test_Fibonnaci, fibArray2);

ZEPHIR_INIT_FUNCS(test_fibonnaci_method_entry) {
	PHP_ME(Test_Fibonnaci, fibInt, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Fibonnaci, fibDouble, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Fibonnaci, fibArray, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Fibonnaci, fibArray2, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
