
extern zend_class_entry *stub_globals_env_ce;

ZEPHIR_INIT_CLASS(Stub_Globals_Env);

PHP_METHOD(Stub_Globals_Env, read);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_globals_env_read, 0, 0, 1)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
#else
	ZEND_ARG_INFO(0, name)
#endif
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_globals_env_method_entry) {
	PHP_ME(Stub_Globals_Env, read, arginfo_stub_globals_env_read, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
