
extern zend_class_entry *stub_globals_serverrequestfactory_ce;

ZEPHIR_INIT_CLASS(Stub_Globals_ServerRequestFactory);

PHP_METHOD(Stub_Globals_ServerRequestFactory, load);
PHP_METHOD(Stub_Globals_ServerRequestFactory, checkNullArray);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_globals_serverrequestfactory_load, 0, 0, IS_ARRAY, 0)
#if PHP_VERSION_ID >= 80000
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, server, IS_ARRAY, 1, "[]")
#else
	ZEND_ARG_ARRAY_INFO(0, server, 1)
#endif
#if PHP_VERSION_ID >= 80000
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, get, IS_ARRAY, 1, "[]")
#else
	ZEND_ARG_ARRAY_INFO(0, get, 1)
#endif
#if PHP_VERSION_ID >= 80000
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, post, IS_ARRAY, 1, "[]")
#else
	ZEND_ARG_ARRAY_INFO(0, post, 1)
#endif
#if PHP_VERSION_ID >= 80000
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, cookies, IS_ARRAY, 1, "[]")
#else
	ZEND_ARG_ARRAY_INFO(0, cookies, 1)
#endif
#if PHP_VERSION_ID >= 80000
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, files, IS_ARRAY, 1, "[]")
#else
	ZEND_ARG_ARRAY_INFO(0, files, 1)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_globals_serverrequestfactory_checknullarray, 0, 2, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, source)
	ZEND_ARG_ARRAY_INFO(0, super, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_globals_serverrequestfactory_method_entry) {
	PHP_ME(Stub_Globals_ServerRequestFactory, load, arginfo_stub_globals_serverrequestfactory_load, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Globals_ServerRequestFactory, checkNullArray, arginfo_stub_globals_serverrequestfactory_checknullarray, ZEND_ACC_PRIVATE)
	PHP_FE_END
};
