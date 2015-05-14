
extern zend_class_entry *test_optimizers_math_ce;

ZEPHIR_INIT_CLASS(Test_Optimizers_Math);

PHP_METHOD(Test_Optimizers_Math, testSqrtInt);
PHP_METHOD(Test_Optimizers_Math, testSqrtVar);
PHP_METHOD(Test_Optimizers_Math, testSqrtIntValue1);
PHP_METHOD(Test_Optimizers_Math, testSqrtIntValue2);
PHP_METHOD(Test_Optimizers_Math, testSqrtIntParameter);
PHP_METHOD(Test_Optimizers_Math, testSqrtVarParameter);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_optimizers_math_testsqrtintparameter, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_optimizers_math_testsqrtvarparameter, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_optimizers_math_method_entry) {
	PHP_ME(Test_Optimizers_Math, testSqrtInt, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Optimizers_Math, testSqrtVar, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Optimizers_Math, testSqrtIntValue1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Optimizers_Math, testSqrtIntValue2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Optimizers_Math, testSqrtIntParameter, arginfo_test_optimizers_math_testsqrtintparameter, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Optimizers_Math, testSqrtVarParameter, arginfo_test_optimizers_math_testsqrtvarparameter, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
