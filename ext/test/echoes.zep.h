
extern zend_class_entry *test_echoes_ce;

ZEPHIR_INIT_CLASS(Test_Echoes);

PHP_METHOD(Test_Echoes, testEcho1);
PHP_METHOD(Test_Echoes, testEcho2);
PHP_METHOD(Test_Echoes, testEcho3);
PHP_METHOD(Test_Echoes, testEcho4);
PHP_METHOD(Test_Echoes, testEcho5);

ZEPHIR_INIT_FUNCS(test_echoes_method_entry) {
	PHP_ME(Test_Echoes, testEcho1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Echoes, testEcho2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Echoes, testEcho3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Echoes, testEcho4, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Echoes, testEcho5, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
