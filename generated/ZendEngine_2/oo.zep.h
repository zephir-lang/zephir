
extern zend_class_entry *test_oo_ce;

ZEPHIR_INIT_CLASS(Test_Oo);

PHP_METHOD(Test_Oo, testInstance1);
PHP_METHOD(Test_Oo, testInstance2);
PHP_METHOD(Test_Oo, testInstance3);
PHP_METHOD(Test_Oo, testInstance4);
PHP_METHOD(Test_Oo, testInstance5);
PHP_METHOD(Test_Oo, testInstance6);
PHP_METHOD(Test_Oo, testInstance7);
PHP_METHOD(Test_Oo, testInstance8);
PHP_METHOD(Test_Oo, testInstance9);
PHP_METHOD(Test_Oo, testInstance10);
PHP_METHOD(Test_Oo, testInstance11);

ZEPHIR_INIT_FUNCS(test_oo_method_entry) {
	PHP_ME(Test_Oo, testInstance1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo, testInstance2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo, testInstance3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo, testInstance4, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo, testInstance5, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo, testInstance6, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo, testInstance7, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo, testInstance8, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo, testInstance9, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo, testInstance10, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo, testInstance11, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
