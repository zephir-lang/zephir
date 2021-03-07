
extern zend_class_entry *stub_optimizers_cos_ce;

ZEPHIR_INIT_CLASS(Stub_Optimizers_Cos);

PHP_METHOD(Stub_Optimizers_Cos, testInt);
PHP_METHOD(Stub_Optimizers_Cos, testVar);
PHP_METHOD(Stub_Optimizers_Cos, testIntValue1);
PHP_METHOD(Stub_Optimizers_Cos, testIntValue2);
PHP_METHOD(Stub_Optimizers_Cos, testIntParameter);
PHP_METHOD(Stub_Optimizers_Cos, testVarParameter);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_cos_testint, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_cos_testvar, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_cos_testintvalue1, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_cos_testintvalue2, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_cos_testintparameter, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_cos_testvarparameter, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_optimizers_cos_method_entry) {
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Optimizers_Cos, testInt, arginfo_stub_optimizers_cos_testint, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Optimizers_Cos, testInt, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Optimizers_Cos, testVar, arginfo_stub_optimizers_cos_testvar, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Optimizers_Cos, testVar, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Optimizers_Cos, testIntValue1, arginfo_stub_optimizers_cos_testintvalue1, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Optimizers_Cos, testIntValue1, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Optimizers_Cos, testIntValue2, arginfo_stub_optimizers_cos_testintvalue2, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Optimizers_Cos, testIntValue2, NULL, ZEND_ACC_PUBLIC)
#endif
	PHP_ME(Stub_Optimizers_Cos, testIntParameter, arginfo_stub_optimizers_cos_testintparameter, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Optimizers_Cos, testVarParameter, arginfo_stub_optimizers_cos_testvarparameter, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
