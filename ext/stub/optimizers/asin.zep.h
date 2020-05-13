
extern zend_class_entry *stub_optimizers_asin_ce;

ZEPHIR_INIT_CLASS(Stub_Optimizers_ASin);

PHP_METHOD(Stub_Optimizers_ASin, testInt);
PHP_METHOD(Stub_Optimizers_ASin, testVar);
PHP_METHOD(Stub_Optimizers_ASin, testIntValue1);
PHP_METHOD(Stub_Optimizers_ASin, testIntParameter);
PHP_METHOD(Stub_Optimizers_ASin, testVarParameter);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_asin_testintparameter, 0, 0, 1)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, a)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_asin_testvarparameter, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_optimizers_asin_method_entry) {
	PHP_ME(Stub_Optimizers_ASin, testInt, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Optimizers_ASin, testVar, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Optimizers_ASin, testIntValue1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Optimizers_ASin, testIntParameter, arginfo_stub_optimizers_asin_testintparameter, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Optimizers_ASin, testVarParameter, arginfo_stub_optimizers_asin_testvarparameter, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
