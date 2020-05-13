
extern zend_class_entry *stub_globals_server_ce;

ZEPHIR_INIT_CLASS(Stub_Globals_Server);

PHP_METHOD(Stub_Globals_Server, f1);
PHP_METHOD(Stub_Globals_Server, f2);
PHP_METHOD(Stub_Globals_Server, check);

#if PHP_VERSION_ID >= 70100
#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_globals_server_f1, 0, 0, IS_VOID, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_globals_server_f1, 0, 0, IS_VOID, NULL, 0)
#endif
ZEND_END_ARG_INFO()
#else
#define arginfo_stub_globals_server_f1 NULL
#endif

#if PHP_VERSION_ID >= 70100
#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_globals_server_f2, 0, 0, IS_VOID, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_globals_server_f2, 0, 0, IS_VOID, NULL, 0)
#endif
ZEND_END_ARG_INFO()
#else
#define arginfo_stub_globals_server_f2 NULL
#endif

ZEPHIR_INIT_FUNCS(stub_globals_server_method_entry) {
	PHP_ME(Stub_Globals_Server, f1, arginfo_stub_globals_server_f1, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Globals_Server, f2, arginfo_stub_globals_server_f2, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Globals_Server, check, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
