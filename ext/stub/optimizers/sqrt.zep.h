
extern zend_class_entry *stub_optimizers_sqrt_ce;

ZEPHIR_INIT_CLASS(Stub_Optimizers_Sqrt);

PHP_METHOD(Stub_Optimizers_Sqrt, testInt);
PHP_METHOD(Stub_Optimizers_Sqrt, testVar);
PHP_METHOD(Stub_Optimizers_Sqrt, testIntValue1);
PHP_METHOD(Stub_Optimizers_Sqrt, testIntValue2);
PHP_METHOD(Stub_Optimizers_Sqrt, testIntParameter);
PHP_METHOD(Stub_Optimizers_Sqrt, testVarParameter);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_sqrt_testint, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_sqrt_testvar, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_sqrt_testintvalue1, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_sqrt_testintvalue2, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_sqrt_testintparameter, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_sqrt_testvarparameter, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_optimizers_sqrt_method_entry) {
PHP_ME(Stub_Optimizers_Sqrt, testInt, arginfo_stub_optimizers_sqrt_testint, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Optimizers_Sqrt, testVar, arginfo_stub_optimizers_sqrt_testvar, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Optimizers_Sqrt, testIntValue1, arginfo_stub_optimizers_sqrt_testintvalue1, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Optimizers_Sqrt, testIntValue2, arginfo_stub_optimizers_sqrt_testintvalue2, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Optimizers_Sqrt, testIntParameter, arginfo_stub_optimizers_sqrt_testintparameter, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Optimizers_Sqrt, testVarParameter, arginfo_stub_optimizers_sqrt_testvarparameter, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
