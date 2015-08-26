
extern zend_class_entry *test_optimizers_isscalar_ce;

ZEPHIR_INIT_CLASS(Test_Optimizers_IsScalar);

PHP_METHOD(Test_Optimizers_IsScalar, testIntVar);
PHP_METHOD(Test_Optimizers_IsScalar, testDoubleVar);
PHP_METHOD(Test_Optimizers_IsScalar, testBoolVar);
PHP_METHOD(Test_Optimizers_IsScalar, testStringVar);
PHP_METHOD(Test_Optimizers_IsScalar, testEmptyArrayVar);
PHP_METHOD(Test_Optimizers_IsScalar, testVar);
PHP_METHOD(Test_Optimizers_IsScalar, testVarParameter);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_optimizers_isscalar_testvarparameter, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_optimizers_isscalar_method_entry) {
	PHP_ME(Test_Optimizers_IsScalar, testIntVar, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Optimizers_IsScalar, testDoubleVar, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Optimizers_IsScalar, testBoolVar, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Optimizers_IsScalar, testStringVar, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Optimizers_IsScalar, testEmptyArrayVar, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Optimizers_IsScalar, testVar, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Optimizers_IsScalar, testVarParameter, arginfo_test_optimizers_isscalar_testvarparameter, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
