
extern zend_class_entry *test_optimizers_sin_ce;

ZEPHIR_INIT_CLASS(Test_Optimizers_Sin);

PHP_METHOD(Test_Optimizers_Sin, testInt);
PHP_METHOD(Test_Optimizers_Sin, testVar);
PHP_METHOD(Test_Optimizers_Sin, testIntValue1);
PHP_METHOD(Test_Optimizers_Sin, testIntValue2);
PHP_METHOD(Test_Optimizers_Sin, testIntParameter);
PHP_METHOD(Test_Optimizers_Sin, testVarParameter);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_optimizers_sin_testintparameter, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_optimizers_sin_testvarparameter, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_optimizers_sin_method_entry) {
	PHP_ME(Test_Optimizers_Sin, testInt, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Optimizers_Sin, testVar, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Optimizers_Sin, testIntValue1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Optimizers_Sin, testIntValue2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Optimizers_Sin, testIntParameter, arginfo_test_optimizers_sin_testintparameter, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Optimizers_Sin, testVarParameter, arginfo_test_optimizers_sin_testvarparameter, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
