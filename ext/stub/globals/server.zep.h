
extern zend_class_entry *stub_globals_server_ce;

ZEPHIR_INIT_CLASS(Stub_Globals_Server);

PHP_METHOD(Stub_Globals_Server, f1);
PHP_METHOD(Stub_Globals_Server, f2);
PHP_METHOD(Stub_Globals_Server, check);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_globals_server_f1, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_globals_server_f2, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_globals_server_check, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_globals_server_method_entry) {
	PHP_ME(Stub_Globals_Server, f1, arginfo_stub_globals_server_f1, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Globals_Server, f2, arginfo_stub_globals_server_f2, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Globals_Server, check, arginfo_stub_globals_server_check, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
