
extern zend_class_entry *test_closures_ce;

ZEPHIR_INIT_CLASS(Test_Closures);

PHP_METHOD(Test_Closures, simple1);
PHP_METHOD(Test_Closures, simple2);
PHP_METHOD(Test_Closures, simple3);
PHP_METHOD(Test_Closures, simple4);
PHP_METHOD(Test_Closures, simple5);
PHP_METHOD(Test_Closures, arrow1);
PHP_METHOD(Test_Closures, arrow2);

ZEPHIR_INIT_FUNCS(test_closures_method_entry) {
	PHP_ME(Test_Closures, simple1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Closures, simple2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Closures, simple3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Closures, simple4, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Closures, simple5, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Closures, arrow1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Closures, arrow2, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
