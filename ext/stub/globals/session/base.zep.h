
extern zend_class_entry *stub_globals_session_base_ce;

ZEPHIR_INIT_CLASS(Stub_Globals_Session_Base);

PHP_METHOD(Stub_Globals_Session_Base, set);
PHP_METHOD(Stub_Globals_Session_Base, remove);
PHP_METHOD(Stub_Globals_Session_Base, __set);
PHP_METHOD(Stub_Globals_Session_Base, __unset);
PHP_METHOD(Stub_Globals_Session_Base, removeSessionData);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_globals_session_base_set, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, index, IS_STRING, 0)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_globals_session_base_remove, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, index, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_globals_session_base___set, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, index, IS_STRING, 0)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_globals_session_base___unset, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, index, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_globals_session_base_removesessiondata, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_globals_session_base_method_entry) {
	PHP_ME(Stub_Globals_Session_Base, set, arginfo_stub_globals_session_base_set, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Globals_Session_Base, remove, arginfo_stub_globals_session_base_remove, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Globals_Session_Base, __set, arginfo_stub_globals_session_base___set, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Globals_Session_Base, __unset, arginfo_stub_globals_session_base___unset, ZEND_ACC_PUBLIC)
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Globals_Session_Base, removeSessionData, arginfo_stub_globals_session_base_removesessiondata, ZEND_ACC_PROTECTED)
#else
	PHP_ME(Stub_Globals_Session_Base, removeSessionData, NULL, ZEND_ACC_PROTECTED)
#endif
	PHP_FE_END
};
