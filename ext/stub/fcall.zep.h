
extern zend_class_entry *stub_fcall_ce;

ZEPHIR_INIT_CLASS(Stub_Fcall);

PHP_METHOD(Stub_Fcall, testCall1);
PHP_METHOD(Stub_Fcall, testCall2);
PHP_METHOD(Stub_Fcall, testCall3);
PHP_METHOD(Stub_Fcall, testCall4);
PHP_METHOD(Stub_Fcall, testCall5);
PHP_METHOD(Stub_Fcall, testCall6);
PHP_METHOD(Stub_Fcall, testCall7);
PHP_METHOD(Stub_Fcall, zvalFcallWith1Parameter);
PHP_METHOD(Stub_Fcall, testCall8);
PHP_METHOD(Stub_Fcall, testCall1FromVar);
PHP_METHOD(Stub_Fcall, testStrtokFalse);
PHP_METHOD(Stub_Fcall, testStrtokVarBySlash);
PHP_METHOD(Stub_Fcall, testFunctionGetArgs);
PHP_METHOD(Stub_Fcall, testFunctionGetArgsAllExtra);
PHP_METHOD(Stub_Fcall, testStaticFunctionGetArgsAllExtra);
PHP_METHOD(Stub_Fcall, testFunctionGetArg);
PHP_METHOD(Stub_Fcall, testFunctionGetArgAllExtra);
PHP_METHOD(Stub_Fcall, testStaticFunctionGetArgAllExtra);
PHP_METHOD(Stub_Fcall, testArrayFill);

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_fcall_testcall1, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_fcall_testcall1, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_fcall_testcall2, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_fcall_testcall2, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_fcall_testcall5, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_fcall_zvalfcallwith1parameter, 0, 0, 1)
	ZEND_ARG_INFO(0, callback)
	ZEND_ARG_INFO(0, param1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_fcall_testcall8, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_fcall_testcall1fromvar, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_fcall_testcall1fromvar, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_fcall_teststrtokfalse, 0, 0, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_fcall_teststrtokfalse, 0, 0, _IS_BOOL, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_fcall_teststrtokvarbyslash, 0, 1, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_fcall_teststrtokvarbyslash, 0, 1, IS_STRING, NULL, 0)
#endif
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_fcall_testfunctiongetargs, 0, 2, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_fcall_testfunctiongetargs, 0, 2, IS_ARRAY, NULL, 0)
#endif
	ZEND_ARG_INFO(0, param1)
	ZEND_ARG_INFO(0, param2)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_fcall_testfunctiongetargsallextra, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_fcall_testfunctiongetargsallextra, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_fcall_teststaticfunctiongetargsallextra, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_fcall_teststaticfunctiongetargsallextra, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_fcall_testfunctiongetarg, 0, 2, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_fcall_testfunctiongetarg, 0, 2, IS_ARRAY, NULL, 0)
#endif
	ZEND_ARG_INFO(0, param1)
	ZEND_ARG_INFO(0, param2)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_fcall_testfunctiongetargallextra, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_fcall_testfunctiongetargallextra, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_fcall_teststaticfunctiongetargallextra, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_fcall_teststaticfunctiongetargallextra, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_fcall_testarrayfill, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_fcall_testarrayfill, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_fcall_method_entry) {
	PHP_ME(Stub_Fcall, testCall1, arginfo_stub_fcall_testcall1, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Fcall, testCall2, arginfo_stub_fcall_testcall2, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Fcall, testCall3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Fcall, testCall4, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Fcall, testCall5, arginfo_stub_fcall_testcall5, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Fcall, testCall6, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Fcall, testCall7, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Fcall, zvalFcallWith1Parameter, arginfo_stub_fcall_zvalfcallwith1parameter, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Fcall, testCall8, arginfo_stub_fcall_testcall8, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Fcall, testCall1FromVar, arginfo_stub_fcall_testcall1fromvar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Fcall, testStrtokFalse, arginfo_stub_fcall_teststrtokfalse, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Fcall, testStrtokVarBySlash, arginfo_stub_fcall_teststrtokvarbyslash, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Fcall, testFunctionGetArgs, arginfo_stub_fcall_testfunctiongetargs, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Fcall, testFunctionGetArgsAllExtra, arginfo_stub_fcall_testfunctiongetargsallextra, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Fcall, testStaticFunctionGetArgsAllExtra, arginfo_stub_fcall_teststaticfunctiongetargsallextra, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Fcall, testFunctionGetArg, arginfo_stub_fcall_testfunctiongetarg, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Fcall, testFunctionGetArgAllExtra, arginfo_stub_fcall_testfunctiongetargallextra, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Fcall, testStaticFunctionGetArgAllExtra, arginfo_stub_fcall_teststaticfunctiongetargallextra, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Fcall, testArrayFill, arginfo_stub_fcall_testarrayfill, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
