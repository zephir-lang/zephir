
extern zend_class_entry *test_optimizers_ldexp_ce;

ZEPHIR_INIT_CLASS(Test_Optimizers_Ldexp);

PHP_METHOD(Test_Optimizers_Ldexp, testInt);
PHP_METHOD(Test_Optimizers_Ldexp, testDoubleInt);
PHP_METHOD(Test_Optimizers_Ldexp, testDouble);
PHP_METHOD(Test_Optimizers_Ldexp, testVar);
PHP_METHOD(Test_Optimizers_Ldexp, testIntValue1);
PHP_METHOD(Test_Optimizers_Ldexp, testIntParameter);
PHP_METHOD(Test_Optimizers_Ldexp, testVarParameter);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_optimizers_ldexp_testintparameter, 0, 0, 2)
	ZEND_ARG_INFO(0, x)
	ZEND_ARG_INFO(0, exponent)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_optimizers_ldexp_testvarparameter, 0, 0, 2)
	ZEND_ARG_INFO(0, x)
	ZEND_ARG_INFO(0, exponent)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_optimizers_ldexp_method_entry) {
	PHP_ME(Test_Optimizers_Ldexp, testInt, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Optimizers_Ldexp, testDoubleInt, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Optimizers_Ldexp, testDouble, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Optimizers_Ldexp, testVar, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Optimizers_Ldexp, testIntValue1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Optimizers_Ldexp, testIntParameter, arginfo_test_optimizers_ldexp_testintparameter, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Optimizers_Ldexp, testVarParameter, arginfo_test_optimizers_ldexp_testvarparameter, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
