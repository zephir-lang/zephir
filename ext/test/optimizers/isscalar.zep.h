
extern zend_class_entry *test_optimizers_isscalar_ce;

ZEPHIR_INIT_CLASS(Test_Optimizers_IsScalar);

PHP_METHOD(Test_Optimizers_IsScalar, testIntVar);
PHP_METHOD(Test_Optimizers_IsScalar, testDoubleVar);
PHP_METHOD(Test_Optimizers_IsScalar, testBoolVar);
PHP_METHOD(Test_Optimizers_IsScalar, testStringVar);
PHP_METHOD(Test_Optimizers_IsScalar, testEmptyArrayVar);
PHP_METHOD(Test_Optimizers_IsScalar, testVar);
PHP_METHOD(Test_Optimizers_IsScalar, testVarParameter);

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_optimizers_isscalar_testintvar, 0, 0, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_optimizers_isscalar_testintvar, 0, 0, _IS_BOOL, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_optimizers_isscalar_testdoublevar, 0, 0, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_optimizers_isscalar_testdoublevar, 0, 0, _IS_BOOL, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_optimizers_isscalar_testboolvar, 0, 0, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_optimizers_isscalar_testboolvar, 0, 0, _IS_BOOL, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_optimizers_isscalar_teststringvar, 0, 0, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_optimizers_isscalar_teststringvar, 0, 0, _IS_BOOL, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_optimizers_isscalar_testemptyarrayvar, 0, 0, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_optimizers_isscalar_testemptyarrayvar, 0, 0, _IS_BOOL, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_optimizers_isscalar_testvar, 0, 0, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_optimizers_isscalar_testvar, 0, 0, _IS_BOOL, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_optimizers_isscalar_testvarparameter, 0, 1, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_optimizers_isscalar_testvarparameter, 0, 1, _IS_BOOL, NULL, 0)
#endif
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_optimizers_isscalar_method_entry) {
	PHP_ME(Test_Optimizers_IsScalar, testIntVar, arginfo_test_optimizers_isscalar_testintvar, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Optimizers_IsScalar, testDoubleVar, arginfo_test_optimizers_isscalar_testdoublevar, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Optimizers_IsScalar, testBoolVar, arginfo_test_optimizers_isscalar_testboolvar, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Optimizers_IsScalar, testStringVar, arginfo_test_optimizers_isscalar_teststringvar, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Optimizers_IsScalar, testEmptyArrayVar, arginfo_test_optimizers_isscalar_testemptyarrayvar, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Optimizers_IsScalar, testVar, arginfo_test_optimizers_isscalar_testvar, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Optimizers_IsScalar, testVarParameter, arginfo_test_optimizers_isscalar_testvarparameter, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
