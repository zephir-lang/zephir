
extern zend_class_entry *test_fcall_ce;

ZEPHIR_INIT_CLASS(Test_Fcall);

PHP_METHOD(Test_Fcall, testCall1);
PHP_METHOD(Test_Fcall, testCall2);
PHP_METHOD(Test_Fcall, testCall3);
PHP_METHOD(Test_Fcall, testCall4);
PHP_METHOD(Test_Fcall, testCall5);
PHP_METHOD(Test_Fcall, testCall6);
PHP_METHOD(Test_Fcall, testCall7);
PHP_METHOD(Test_Fcall, zvalFcallWith1Parameter);
PHP_METHOD(Test_Fcall, testCall8);
PHP_METHOD(Test_Fcall, testCall1FromVar);
PHP_METHOD(Test_Fcall, testStrtokFalse);
PHP_METHOD(Test_Fcall, testStrtokVarBySlash);
PHP_METHOD(Test_Fcall, testFunctionGetArgs);
PHP_METHOD(Test_Fcall, testArrayFill);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_fcall_testcall5, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_fcall_zvalfcallwith1parameter, 0, 0, 1)
	ZEND_ARG_INFO(0, callback)
	ZEND_ARG_INFO(0, param1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_fcall_testcall8, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_fcall_teststrtokvarbyslash, 0, 0, 1)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_fcall_testfunctiongetargs, 0, 0, 2)
	ZEND_ARG_INFO(0, param1)
	ZEND_ARG_INFO(0, param2)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_fcall_method_entry) {
	PHP_ME(Test_Fcall, testCall1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Fcall, testCall2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Fcall, testCall3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Fcall, testCall4, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Fcall, testCall5, arginfo_test_fcall_testcall5, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Fcall, testCall6, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Fcall, testCall7, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Fcall, zvalFcallWith1Parameter, arginfo_test_fcall_zvalfcallwith1parameter, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Fcall, testCall8, arginfo_test_fcall_testcall8, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Fcall, testCall1FromVar, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Fcall, testStrtokFalse, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Fcall, testStrtokVarBySlash, arginfo_test_fcall_teststrtokvarbyslash, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Fcall, testFunctionGetArgs, arginfo_test_fcall_testfunctiongetargs, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Fcall, testArrayFill, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
