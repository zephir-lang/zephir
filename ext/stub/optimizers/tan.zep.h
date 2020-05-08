
extern zend_class_entry *stub_optimizers_tan_ce;

ZEPHIR_INIT_CLASS(Stub_Optimizers_Tan);

PHP_METHOD(Stub_Optimizers_Tan, testInt);
PHP_METHOD(Stub_Optimizers_Tan, testVar);
PHP_METHOD(Stub_Optimizers_Tan, testVar2);
PHP_METHOD(Stub_Optimizers_Tan, testIntValue1);
PHP_METHOD(Stub_Optimizers_Tan, testIntValue2);
PHP_METHOD(Stub_Optimizers_Tan, testIntParameter);
PHP_METHOD(Stub_Optimizers_Tan, testVarParameter);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_tan_testintparameter, 0, 0, 1)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, a)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_tan_testvarparameter, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_optimizers_tan_method_entry) {
	PHP_ME(Stub_Optimizers_Tan, testInt, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Optimizers_Tan, testVar, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Optimizers_Tan, testVar2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Optimizers_Tan, testIntValue1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Optimizers_Tan, testIntValue2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Optimizers_Tan, testIntParameter, arginfo_stub_optimizers_tan_testintparameter, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Optimizers_Tan, testVarParameter, arginfo_stub_optimizers_tan_testvarparameter, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
