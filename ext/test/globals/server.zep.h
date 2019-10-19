
extern zend_class_entry *test_globals_server_ce;

ZEPHIR_INIT_CLASS(Test_Globals_Server);

PHP_METHOD(Test_Globals_Server, f1);
PHP_METHOD(Test_Globals_Server, f2);
PHP_METHOD(Test_Globals_Server, check);

#if PHP_VERSION_ID >= 70100
#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_globals_server_f1, 0, 0, IS_VOID, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_globals_server_f1, 0, 0, IS_VOID, NULL, 0)
#endif
ZEND_END_ARG_INFO()
#else
#define arginfo_test_globals_server_f1 NULL
#endif

#if PHP_VERSION_ID >= 70100
#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_globals_server_f2, 0, 0, IS_VOID, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_globals_server_f2, 0, 0, IS_VOID, NULL, 0)
#endif
ZEND_END_ARG_INFO()
#else
#define arginfo_test_globals_server_f2 NULL
#endif

ZEPHIR_INIT_FUNCS(test_globals_server_method_entry) {
	PHP_ME(Test_Globals_Server, f1, arginfo_test_globals_server_f1, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Globals_Server, f2, arginfo_test_globals_server_f2, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Globals_Server, check, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
