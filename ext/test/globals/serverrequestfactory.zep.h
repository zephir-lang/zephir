
extern zend_class_entry *test_globals_serverrequestfactory_ce;

ZEPHIR_INIT_CLASS(Test_Globals_ServerRequestFactory);

PHP_METHOD(Test_Globals_ServerRequestFactory, load);
PHP_METHOD(Test_Globals_ServerRequestFactory, checkNullArray);

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_globals_serverrequestfactory_load, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_globals_serverrequestfactory_load, 0, 0, IS_ARRAY, NULL, 0)
#endif
	ZEND_ARG_ARRAY_INFO(0, server, 1)
	ZEND_ARG_ARRAY_INFO(0, get, 1)
	ZEND_ARG_ARRAY_INFO(0, post, 1)
	ZEND_ARG_ARRAY_INFO(0, cookies, 1)
	ZEND_ARG_ARRAY_INFO(0, files, 1)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_globals_serverrequestfactory_checknullarray, 0, 2, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_globals_serverrequestfactory_checknullarray, 0, 2, IS_ARRAY, NULL, 0)
#endif
	ZEND_ARG_INFO(0, source)
	ZEND_ARG_ARRAY_INFO(0, super, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_globals_serverrequestfactory_method_entry) {
	PHP_ME(Test_Globals_ServerRequestFactory, load, arginfo_test_globals_serverrequestfactory_load, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Globals_ServerRequestFactory, checkNullArray, arginfo_test_globals_serverrequestfactory_checknullarray, ZEND_ACC_PRIVATE)
	PHP_FE_END
};
