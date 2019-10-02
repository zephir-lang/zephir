
extern zend_class_entry *test_globals_ce;

ZEPHIR_INIT_CLASS(Test_Globals);

PHP_METHOD(Test_Globals, setBoolValueUsingDotNotation);
PHP_METHOD(Test_Globals, setIntValueUsingDotNotation);
PHP_METHOD(Test_Globals, setCharValue);
PHP_METHOD(Test_Globals, setBoolValue);
PHP_METHOD(Test_Globals, setDefaultGlobalsOrmCacheLevel);
PHP_METHOD(Test_Globals, getDefaultGlobals1);
PHP_METHOD(Test_Globals, getDefaultGlobals2);
PHP_METHOD(Test_Globals, getDefaultGlobals3);
PHP_METHOD(Test_Globals, getDefaultGlobals4);
PHP_METHOD(Test_Globals, getDefaultGlobals5);
PHP_METHOD(Test_Globals, getDefaultGlobals6);
PHP_METHOD(Test_Globals, getDefaultGlobals7);
PHP_METHOD(Test_Globals, getDefaultGlobalsOrmCacheLevel);

#if PHP_VERSION_ID >= 70100
#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_globals_setboolvalueusingdotnotation, 0, 1, IS_VOID, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_globals_setboolvalueusingdotnotation, 0, 1, IS_VOID, NULL, 0)
#endif
#else
ZEND_BEGIN_ARG_INFO_EX(arginfo_test_globals_setboolvalueusingdotnotation, 0, 0, 1)
#define arginfo_test_globals_setboolvalueusingdotnotation NULL
#endif

	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70100
#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_globals_setintvalueusingdotnotation, 0, 1, IS_VOID, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_globals_setintvalueusingdotnotation, 0, 1, IS_VOID, NULL, 0)
#endif
#else
ZEND_BEGIN_ARG_INFO_EX(arginfo_test_globals_setintvalueusingdotnotation, 0, 0, 1)
#define arginfo_test_globals_setintvalueusingdotnotation NULL
#endif

	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70100
#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_globals_setcharvalue, 0, 1, IS_VOID, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_globals_setcharvalue, 0, 1, IS_VOID, NULL, 0)
#endif
#else
ZEND_BEGIN_ARG_INFO_EX(arginfo_test_globals_setcharvalue, 0, 0, 1)
#define arginfo_test_globals_setcharvalue NULL
#endif

	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70100
#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_globals_setboolvalue, 0, 1, IS_VOID, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_globals_setboolvalue, 0, 1, IS_VOID, NULL, 0)
#endif
#else
ZEND_BEGIN_ARG_INFO_EX(arginfo_test_globals_setboolvalue, 0, 0, 1)
#define arginfo_test_globals_setboolvalue NULL
#endif

	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70100
#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_globals_setdefaultglobalsormcachelevel, 0, 1, IS_VOID, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_globals_setdefaultglobalsormcachelevel, 0, 1, IS_VOID, NULL, 0)
#endif
#else
ZEND_BEGIN_ARG_INFO_EX(arginfo_test_globals_setdefaultglobalsormcachelevel, 0, 0, 1)
#define arginfo_test_globals_setdefaultglobalsormcachelevel NULL
#endif

	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_globals_method_entry) {
	PHP_ME(Test_Globals, setBoolValueUsingDotNotation, arginfo_test_globals_setboolvalueusingdotnotation, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Globals, setIntValueUsingDotNotation, arginfo_test_globals_setintvalueusingdotnotation, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Globals, setCharValue, arginfo_test_globals_setcharvalue, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Globals, setBoolValue, arginfo_test_globals_setboolvalue, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Globals, setDefaultGlobalsOrmCacheLevel, arginfo_test_globals_setdefaultglobalsormcachelevel, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Globals, getDefaultGlobals1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Globals, getDefaultGlobals2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Globals, getDefaultGlobals3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Globals, getDefaultGlobals4, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Globals, getDefaultGlobals5, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Globals, getDefaultGlobals6, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Globals, getDefaultGlobals7, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Globals, getDefaultGlobalsOrmCacheLevel, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
