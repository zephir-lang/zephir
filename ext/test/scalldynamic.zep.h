
extern zend_class_entry *test_scalldynamic_ce;

ZEPHIR_INIT_CLASS(Test_ScallDynamic);

PHP_METHOD(Test_ScallDynamic, testMethod1);
PHP_METHOD(Test_ScallDynamic, testMethod2);
PHP_METHOD(Test_ScallDynamic, testMethod3);
PHP_METHOD(Test_ScallDynamic, selfDynamicCall1);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_scalldynamic_selfdynamiccall1, 0, 0, 1)
	ZEND_ARG_INFO(0, methodName)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_scalldynamic_method_entry) {
	PHP_ME(Test_ScallDynamic, testMethod1, NULL, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Test_ScallDynamic, testMethod2, NULL, ZEND_ACC_STATIC|ZEND_ACC_PROTECTED)
	PHP_ME(Test_ScallDynamic, testMethod3, NULL, ZEND_ACC_STATIC|ZEND_ACC_PRIVATE)
	PHP_ME(Test_ScallDynamic, selfDynamicCall1, arginfo_test_scalldynamic_selfdynamiccall1, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_FE_END
};
