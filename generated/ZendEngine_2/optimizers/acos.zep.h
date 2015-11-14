
extern zend_class_entry *test_optimizers_acos_ce;

ZEPHIR_INIT_CLASS(Test_Optimizers_ACos);

PHP_METHOD(Test_Optimizers_ACos, testInt);
PHP_METHOD(Test_Optimizers_ACos, testVar);
PHP_METHOD(Test_Optimizers_ACos, testIntValue1);
PHP_METHOD(Test_Optimizers_ACos, testIntParameter);
PHP_METHOD(Test_Optimizers_ACos, testVarParameter);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_optimizers_acos_testintparameter, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_optimizers_acos_testvarparameter, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_optimizers_acos_method_entry) {
	PHP_ME(Test_Optimizers_ACos, testInt, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Optimizers_ACos, testVar, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Optimizers_ACos, testIntValue1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Optimizers_ACos, testIntParameter, arginfo_test_optimizers_acos_testintparameter, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Optimizers_ACos, testVarParameter, arginfo_test_optimizers_acos_testvarparameter, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
