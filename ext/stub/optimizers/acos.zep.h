
extern zend_class_entry *stub_optimizers_acos_ce;

ZEPHIR_INIT_CLASS(Stub_Optimizers_ACos);

PHP_METHOD(Stub_Optimizers_ACos, testInt);
PHP_METHOD(Stub_Optimizers_ACos, testVar);
PHP_METHOD(Stub_Optimizers_ACos, testIntValue1);
PHP_METHOD(Stub_Optimizers_ACos, testIntParameter);
PHP_METHOD(Stub_Optimizers_ACos, testVarParameter);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_acos_testint, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_acos_testvar, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_acos_testintvalue1, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_acos_testintparameter, 0, 0, 1)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, a)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_acos_testvarparameter, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_optimizers_acos_method_entry) {
	PHP_ME(Stub_Optimizers_ACos, testInt, arginfo_stub_optimizers_acos_testint, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Optimizers_ACos, testVar, arginfo_stub_optimizers_acos_testvar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Optimizers_ACos, testIntValue1, arginfo_stub_optimizers_acos_testintvalue1, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Optimizers_ACos, testIntParameter, arginfo_stub_optimizers_acos_testintparameter, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Optimizers_ACos, testVarParameter, arginfo_stub_optimizers_acos_testvarparameter, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
