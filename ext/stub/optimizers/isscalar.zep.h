
extern zend_class_entry *stub_optimizers_isscalar_ce;

ZEPHIR_INIT_CLASS(Stub_Optimizers_IsScalar);

PHP_METHOD(Stub_Optimizers_IsScalar, testIntVar);
PHP_METHOD(Stub_Optimizers_IsScalar, testDoubleVar);
PHP_METHOD(Stub_Optimizers_IsScalar, testBoolVar);
PHP_METHOD(Stub_Optimizers_IsScalar, testStringVar);
PHP_METHOD(Stub_Optimizers_IsScalar, testEmptyArrayVar);
PHP_METHOD(Stub_Optimizers_IsScalar, testVar);
PHP_METHOD(Stub_Optimizers_IsScalar, testVarParameter);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_optimizers_isscalar_testintvar, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_optimizers_isscalar_testdoublevar, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_optimizers_isscalar_testboolvar, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_optimizers_isscalar_teststringvar, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_optimizers_isscalar_testemptyarrayvar, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_optimizers_isscalar_testvar, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_optimizers_isscalar_testvarparameter, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_optimizers_isscalar_method_entry) {
	PHP_ME(Stub_Optimizers_IsScalar, testIntVar, arginfo_stub_optimizers_isscalar_testintvar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Optimizers_IsScalar, testDoubleVar, arginfo_stub_optimizers_isscalar_testdoublevar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Optimizers_IsScalar, testBoolVar, arginfo_stub_optimizers_isscalar_testboolvar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Optimizers_IsScalar, testStringVar, arginfo_stub_optimizers_isscalar_teststringvar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Optimizers_IsScalar, testEmptyArrayVar, arginfo_stub_optimizers_isscalar_testemptyarrayvar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Optimizers_IsScalar, testVar, arginfo_stub_optimizers_isscalar_testvar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Optimizers_IsScalar, testVarParameter, arginfo_stub_optimizers_isscalar_testvarparameter, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
