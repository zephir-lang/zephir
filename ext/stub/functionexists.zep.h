
extern zend_class_entry *stub_functionexists_ce;

ZEPHIR_INIT_CLASS(Stub_FunctionExists);

PHP_METHOD(Stub_FunctionExists, testWithPassedName);
PHP_METHOD(Stub_FunctionExists, testBuiltInFunctions);
PHP_METHOD(Stub_FunctionExists, testWithString);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_functionexists_testwithpassedname, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, func, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_functionexists_testbuiltinfunctions, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_functionexists_testwithstring, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_functionexists_method_entry) {
	PHP_ME(Stub_FunctionExists, testWithPassedName, arginfo_stub_functionexists_testwithpassedname, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_FunctionExists, testBuiltInFunctions, arginfo_stub_functionexists_testbuiltinfunctions, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_FunctionExists, testWithString, arginfo_stub_functionexists_testwithstring, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
