
extern zend_class_entry *test_globals_env_ce;

ZEPHIR_INIT_CLASS(Test_Globals_Env);

PHP_METHOD(Test_Globals_Env, read);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_globals_env_read, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_globals_env_method_entry) {
	PHP_ME(Test_Globals_Env, read, arginfo_test_globals_env_read, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
