
extern zend_class_entry *stub_optimizers_sin_ce;

ZEPHIR_INIT_CLASS(Stub_Optimizers_Sin);

PHP_METHOD(Stub_Optimizers_Sin, testInt);
PHP_METHOD(Stub_Optimizers_Sin, testVar);
PHP_METHOD(Stub_Optimizers_Sin, testIntValue1);
PHP_METHOD(Stub_Optimizers_Sin, testIntValue2);
PHP_METHOD(Stub_Optimizers_Sin, testIntParameter);
PHP_METHOD(Stub_Optimizers_Sin, testVarParameter);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_sin_testint, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_sin_testvar, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_sin_testintvalue1, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_sin_testintvalue2, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_sin_testintparameter, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_sin_testvarparameter, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_optimizers_sin_method_entry) {
PHP_ME(Stub_Optimizers_Sin, testInt, arginfo_stub_optimizers_sin_testint, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Optimizers_Sin, testVar, arginfo_stub_optimizers_sin_testvar, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Optimizers_Sin, testIntValue1, arginfo_stub_optimizers_sin_testintvalue1, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Optimizers_Sin, testIntValue2, arginfo_stub_optimizers_sin_testintvalue2, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Optimizers_Sin, testIntParameter, arginfo_stub_optimizers_sin_testintparameter, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Optimizers_Sin, testVarParameter, arginfo_stub_optimizers_sin_testvarparameter, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
