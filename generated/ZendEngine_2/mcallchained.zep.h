
extern zend_class_entry *test_mcallchained_ce;

ZEPHIR_INIT_CLASS(Test_McallChained);

PHP_METHOD(Test_McallChained, testMethod1);
PHP_METHOD(Test_McallChained, testMethod2);
PHP_METHOD(Test_McallChained, testMethod3);
PHP_METHOD(Test_McallChained, testChained1);
PHP_METHOD(Test_McallChained, testChained2);
PHP_METHOD(Test_McallChained, testChained3);
PHP_METHOD(Test_McallChained, testChained4);

ZEPHIR_INIT_FUNCS(test_mcallchained_method_entry) {
	PHP_ME(Test_McallChained, testMethod1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_McallChained, testMethod2, NULL, ZEND_ACC_PROTECTED)
	PHP_ME(Test_McallChained, testMethod3, NULL, ZEND_ACC_PRIVATE)
	PHP_ME(Test_McallChained, testChained1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_McallChained, testChained2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_McallChained, testChained3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_McallChained, testChained4, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
