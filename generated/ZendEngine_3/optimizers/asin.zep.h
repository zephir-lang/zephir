
extern zend_class_entry *test_optimizers_asin_ce;

ZEPHIR_INIT_CLASS(Test_Optimizers_ASin);

PHP_METHOD(Test_Optimizers_ASin, testInt);
PHP_METHOD(Test_Optimizers_ASin, testVar);
PHP_METHOD(Test_Optimizers_ASin, testIntValue1);
PHP_METHOD(Test_Optimizers_ASin, testIntParameter);
PHP_METHOD(Test_Optimizers_ASin, testVarParameter);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_optimizers_asin_testintparameter, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_optimizers_asin_testvarparameter, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_optimizers_asin_method_entry) {
	PHP_ME(Test_Optimizers_ASin, testInt, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Optimizers_ASin, testVar, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Optimizers_ASin, testIntValue1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Optimizers_ASin, testIntParameter, arginfo_test_optimizers_asin_testintparameter, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Optimizers_ASin, testVarParameter, arginfo_test_optimizers_asin_testvarparameter, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
