
extern zend_class_entry *stub_optimizers_tan_ce;

ZEPHIR_INIT_CLASS(Stub_Optimizers_Tan);

PHP_METHOD(Stub_Optimizers_Tan, testInt);
PHP_METHOD(Stub_Optimizers_Tan, testVar);
PHP_METHOD(Stub_Optimizers_Tan, testVar2);
PHP_METHOD(Stub_Optimizers_Tan, testIntValue1);
PHP_METHOD(Stub_Optimizers_Tan, testIntValue2);
PHP_METHOD(Stub_Optimizers_Tan, testIntParameter);
PHP_METHOD(Stub_Optimizers_Tan, testVarParameter);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_tan_testint, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_tan_testvar, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_tan_testvar2, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_tan_testintvalue1, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_tan_testintvalue2, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_tan_testintparameter, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_tan_testvarparameter, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_optimizers_tan_method_entry) {
PHP_ME(Stub_Optimizers_Tan, testInt, arginfo_stub_optimizers_tan_testint, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Optimizers_Tan, testVar, arginfo_stub_optimizers_tan_testvar, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Optimizers_Tan, testVar2, arginfo_stub_optimizers_tan_testvar2, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Optimizers_Tan, testIntValue1, arginfo_stub_optimizers_tan_testintvalue1, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Optimizers_Tan, testIntValue2, arginfo_stub_optimizers_tan_testintvalue2, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Optimizers_Tan, testIntParameter, arginfo_stub_optimizers_tan_testintparameter, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Optimizers_Tan, testVarParameter, arginfo_stub_optimizers_tan_testvarparameter, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
