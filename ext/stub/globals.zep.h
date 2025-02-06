
extern zend_class_entry *stub_globals_ce;

ZEPHIR_INIT_CLASS(Stub_Globals);

PHP_METHOD(Stub_Globals, setBoolValueUsingDotNotation);
PHP_METHOD(Stub_Globals, setIntValueUsingDotNotation);
PHP_METHOD(Stub_Globals, setCharValue);
PHP_METHOD(Stub_Globals, setStringValue);
PHP_METHOD(Stub_Globals, setBoolValue);
PHP_METHOD(Stub_Globals, setDefaultGlobalsOrmCacheLevel);
PHP_METHOD(Stub_Globals, setDefaultGlobalsOrmCachePrefix);
PHP_METHOD(Stub_Globals, getDefaultGlobals1);
PHP_METHOD(Stub_Globals, getDefaultGlobals2);
PHP_METHOD(Stub_Globals, getDefaultGlobals3);
PHP_METHOD(Stub_Globals, getDefaultGlobals4);
PHP_METHOD(Stub_Globals, getDefaultGlobals5);
PHP_METHOD(Stub_Globals, getDefaultGlobals6);
PHP_METHOD(Stub_Globals, getDefaultGlobals7);
PHP_METHOD(Stub_Globals, getDefaultGlobals8);
PHP_METHOD(Stub_Globals, getDefaultGlobalsOrmCacheLevel);
PHP_METHOD(Stub_Globals, getDefaultGlobalsOrmCachePrefix);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_globals_setboolvalueusingdotnotation, 0, 1, IS_VOID, 0)

	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_globals_setintvalueusingdotnotation, 0, 1, IS_VOID, 0)

	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_globals_setcharvalue, 0, 1, IS_VOID, 0)

	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_globals_setstringvalue, 0, 1, IS_VOID, 0)

	ZEND_ARG_TYPE_INFO(0, value, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_globals_setboolvalue, 0, 1, IS_VOID, 0)

	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_globals_setdefaultglobalsormcachelevel, 0, 1, IS_VOID, 0)

	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_globals_setdefaultglobalsormcacheprefix, 0, 1, IS_VOID, 0)

	ZEND_ARG_TYPE_INFO(0, value, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_globals_getdefaultglobals1, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_globals_getdefaultglobals2, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_globals_getdefaultglobals3, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_globals_getdefaultglobals4, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_globals_getdefaultglobals5, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_globals_getdefaultglobals6, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_globals_getdefaultglobals7, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_globals_getdefaultglobals8, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_globals_getdefaultglobalsormcachelevel, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_globals_getdefaultglobalsormcacheprefix, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_globals_method_entry) {
	PHP_ME(Stub_Globals, setBoolValueUsingDotNotation, arginfo_stub_globals_setboolvalueusingdotnotation, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Globals, setIntValueUsingDotNotation, arginfo_stub_globals_setintvalueusingdotnotation, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Globals, setCharValue, arginfo_stub_globals_setcharvalue, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Globals, setStringValue, arginfo_stub_globals_setstringvalue, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Globals, setBoolValue, arginfo_stub_globals_setboolvalue, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Globals, setDefaultGlobalsOrmCacheLevel, arginfo_stub_globals_setdefaultglobalsormcachelevel, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Globals, setDefaultGlobalsOrmCachePrefix, arginfo_stub_globals_setdefaultglobalsormcacheprefix, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Globals, getDefaultGlobals1, arginfo_stub_globals_getdefaultglobals1, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Globals, getDefaultGlobals2, arginfo_stub_globals_getdefaultglobals2, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Globals, getDefaultGlobals3, arginfo_stub_globals_getdefaultglobals3, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Globals, getDefaultGlobals4, arginfo_stub_globals_getdefaultglobals4, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Globals, getDefaultGlobals5, arginfo_stub_globals_getdefaultglobals5, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Globals, getDefaultGlobals6, arginfo_stub_globals_getdefaultglobals6, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Globals, getDefaultGlobals7, arginfo_stub_globals_getdefaultglobals7, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Globals, getDefaultGlobals8, arginfo_stub_globals_getdefaultglobals8, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Globals, getDefaultGlobalsOrmCacheLevel, arginfo_stub_globals_getdefaultglobalsormcachelevel, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Globals, getDefaultGlobalsOrmCachePrefix, arginfo_stub_globals_getdefaultglobalsormcacheprefix, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
