
extern zend_class_entry *stub_optimizers_asin_ce;

ZEPHIR_INIT_CLASS(Stub_Optimizers_ASin);

PHP_METHOD(Stub_Optimizers_ASin, testInt);
PHP_METHOD(Stub_Optimizers_ASin, testVar);
PHP_METHOD(Stub_Optimizers_ASin, testIntValue1);
PHP_METHOD(Stub_Optimizers_ASin, testIntParameter);
PHP_METHOD(Stub_Optimizers_ASin, testVarParameter);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_asin_testint, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_asin_testvar, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_asin_testintvalue1, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_asin_testintparameter, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_asin_testvarparameter, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_optimizers_asin_method_entry) {
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Optimizers_ASin, testInt, arginfo_stub_optimizers_asin_testint, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Optimizers_ASin, testInt, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Optimizers_ASin, testVar, arginfo_stub_optimizers_asin_testvar, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Optimizers_ASin, testVar, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Optimizers_ASin, testIntValue1, arginfo_stub_optimizers_asin_testintvalue1, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Optimizers_ASin, testIntValue1, NULL, ZEND_ACC_PUBLIC)
#endif
	PHP_ME(Stub_Optimizers_ASin, testIntParameter, arginfo_stub_optimizers_asin_testintparameter, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Optimizers_ASin, testVarParameter, arginfo_stub_optimizers_asin_testvarparameter, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
