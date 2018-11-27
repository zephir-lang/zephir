
extern zend_class_entry *test_functionexists_ce;

ZEPHIR_INIT_CLASS(Test_FunctionExists);

PHP_METHOD(Test_FunctionExists, testWithPassedName);
PHP_METHOD(Test_FunctionExists, testBuiltInFunctions);
PHP_METHOD(Test_FunctionExists, testWithString);

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_functionexists_testwithpassedname, 0, 1, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_functionexists_testwithpassedname, 0, 1, _IS_BOOL, NULL, 0)
#endif
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, func, IS_STRING, 0)
#else
	ZEND_ARG_INFO(0, func)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_functionexists_testbuiltinfunctions, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_functionexists_testbuiltinfunctions, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_functionexists_testwithstring, 0, 0, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_functionexists_testwithstring, 0, 0, _IS_BOOL, NULL, 0)
#endif
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_functionexists_method_entry) {
	PHP_ME(Test_FunctionExists, testWithPassedName, arginfo_test_functionexists_testwithpassedname, ZEND_ACC_PUBLIC)
	PHP_ME(Test_FunctionExists, testBuiltInFunctions, arginfo_test_functionexists_testbuiltinfunctions, ZEND_ACC_PUBLIC)
	PHP_ME(Test_FunctionExists, testWithString, arginfo_test_functionexists_testwithstring, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
