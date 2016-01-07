
extern zend_class_entry *test_optimizers_cos_ce;

ZEPHIR_INIT_CLASS(Test_Optimizers_Cos);

PHP_METHOD(Test_Optimizers_Cos, testInt);
PHP_METHOD(Test_Optimizers_Cos, testVar);
PHP_METHOD(Test_Optimizers_Cos, testIntValue1);
PHP_METHOD(Test_Optimizers_Cos, testIntValue2);
PHP_METHOD(Test_Optimizers_Cos, testIntParameter);
PHP_METHOD(Test_Optimizers_Cos, testVarParameter);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_optimizers_cos_testintparameter, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_optimizers_cos_testvarparameter, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_optimizers_cos_method_entry) {
	PHP_ME(Test_Optimizers_Cos, testInt, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Optimizers_Cos, testVar, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Optimizers_Cos, testIntValue1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Optimizers_Cos, testIntValue2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Optimizers_Cos, testIntParameter, arginfo_test_optimizers_cos_testintparameter, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Optimizers_Cos, testVarParameter, arginfo_test_optimizers_cos_testvarparameter, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
