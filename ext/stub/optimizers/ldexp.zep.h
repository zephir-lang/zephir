
extern zend_class_entry *stub_optimizers_ldexp_ce;

ZEPHIR_INIT_CLASS(Stub_Optimizers_Ldexp);

PHP_METHOD(Stub_Optimizers_Ldexp, testInt);
PHP_METHOD(Stub_Optimizers_Ldexp, testDoubleInt);
PHP_METHOD(Stub_Optimizers_Ldexp, testDouble);
PHP_METHOD(Stub_Optimizers_Ldexp, testVar);
PHP_METHOD(Stub_Optimizers_Ldexp, testIntValue1);
PHP_METHOD(Stub_Optimizers_Ldexp, testIntParameter);
PHP_METHOD(Stub_Optimizers_Ldexp, testVarParameter);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_ldexp_testint, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_ldexp_testdoubleint, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_ldexp_testdouble, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_ldexp_testvar, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_ldexp_testintvalue1, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_ldexp_testintparameter, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, x, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, exponent, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_ldexp_testvarparameter, 0, 0, 2)
	ZEND_ARG_INFO(0, x)
	ZEND_ARG_INFO(0, exponent)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_optimizers_ldexp_method_entry) {
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Optimizers_Ldexp, testInt, arginfo_stub_optimizers_ldexp_testint, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Optimizers_Ldexp, testInt, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Optimizers_Ldexp, testDoubleInt, arginfo_stub_optimizers_ldexp_testdoubleint, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Optimizers_Ldexp, testDoubleInt, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Optimizers_Ldexp, testDouble, arginfo_stub_optimizers_ldexp_testdouble, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Optimizers_Ldexp, testDouble, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Optimizers_Ldexp, testVar, arginfo_stub_optimizers_ldexp_testvar, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Optimizers_Ldexp, testVar, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Optimizers_Ldexp, testIntValue1, arginfo_stub_optimizers_ldexp_testintvalue1, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Optimizers_Ldexp, testIntValue1, NULL, ZEND_ACC_PUBLIC)
#endif
	PHP_ME(Stub_Optimizers_Ldexp, testIntParameter, arginfo_stub_optimizers_ldexp_testintparameter, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Optimizers_Ldexp, testVarParameter, arginfo_stub_optimizers_ldexp_testvarparameter, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
