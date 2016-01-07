
extern zend_class_entry *test_optimizers_sqrt_ce;

ZEPHIR_INIT_CLASS(Test_Optimizers_Sqrt);

PHP_METHOD(Test_Optimizers_Sqrt, testInt);
PHP_METHOD(Test_Optimizers_Sqrt, testVar);
PHP_METHOD(Test_Optimizers_Sqrt, testIntValue1);
PHP_METHOD(Test_Optimizers_Sqrt, testIntValue2);
PHP_METHOD(Test_Optimizers_Sqrt, testIntParameter);
PHP_METHOD(Test_Optimizers_Sqrt, testVarParameter);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_optimizers_sqrt_testintparameter, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_optimizers_sqrt_testvarparameter, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_optimizers_sqrt_method_entry) {
	PHP_ME(Test_Optimizers_Sqrt, testInt, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Optimizers_Sqrt, testVar, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Optimizers_Sqrt, testIntValue1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Optimizers_Sqrt, testIntValue2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Optimizers_Sqrt, testIntParameter, arginfo_test_optimizers_sqrt_testintparameter, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Optimizers_Sqrt, testVarParameter, arginfo_test_optimizers_sqrt_testvarparameter, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
