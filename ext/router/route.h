
extern zend_class_entry *test_router_route_ce;

ZEPHIR_INIT_CLASS(Test_Router_Route);

PHP_METHOD(Test_Router_Route, __construct);
PHP_METHOD(Test_Router_Route, compilePattern);
PHP_METHOD(Test_Router_Route, via);
PHP_METHOD(Test_Router_Route, extractNamedParams);
PHP_METHOD(Test_Router_Route, reConfigure);
PHP_METHOD(Test_Router_Route, getName);
PHP_METHOD(Test_Router_Route, setName);
PHP_METHOD(Test_Router_Route, beforeMatch);
PHP_METHOD(Test_Router_Route, getBeforeMatch);
PHP_METHOD(Test_Router_Route, getRouteId);
PHP_METHOD(Test_Router_Route, getPattern);
PHP_METHOD(Test_Router_Route, getCompiledPattern);
PHP_METHOD(Test_Router_Route, getPaths);
PHP_METHOD(Test_Router_Route, getReversedPaths);
PHP_METHOD(Test_Router_Route, setHttpMethods);
PHP_METHOD(Test_Router_Route, getHttpMethods);
PHP_METHOD(Test_Router_Route, setHostname);
PHP_METHOD(Test_Router_Route, getHostname);
PHP_METHOD(Test_Router_Route, convert);
PHP_METHOD(Test_Router_Route, getConverters);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_router_route___construct, 0, 0, 0)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, paths)
	ZEND_ARG_INFO(0, httpMethods)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_router_route_compilepattern, 0, 0, 0)
	ZEND_ARG_INFO(0, pattern)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_router_route_via, 0, 0, 0)
	ZEND_ARG_INFO(0, httpMethods)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_router_route_extractnamedparams, 0, 0, 0)
	ZEND_ARG_INFO(0, pattern)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_router_route_reconfigure, 0, 0, 0)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, paths)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_router_route_setname, 0, 0, 0)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_router_route_beforematch, 0, 0, 0)
	ZEND_ARG_INFO(0, callback)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_router_route_sethttpmethods, 0, 0, 0)
	ZEND_ARG_INFO(0, httpMethods)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_router_route_sethostname, 0, 0, 0)
	ZEND_ARG_INFO(0, hostname)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_router_route_convert, 0, 0, 0)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, converter)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_router_route_method_entry) {
	PHP_ME(Test_Router_Route, __construct, arginfo_test_router_route___construct, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router_Route, compilePattern, arginfo_test_router_route_compilePattern, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router_Route, via, arginfo_test_router_route_via, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router_Route, extractNamedParams, arginfo_test_router_route_extractNamedParams, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router_Route, reConfigure, arginfo_test_router_route_reConfigure, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router_Route, getName, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router_Route, setName, arginfo_test_router_route_setName, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router_Route, beforeMatch, arginfo_test_router_route_beforeMatch, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router_Route, getBeforeMatch, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router_Route, getRouteId, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router_Route, getPattern, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router_Route, getCompiledPattern, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router_Route, getPaths, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router_Route, getReversedPaths, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router_Route, setHttpMethods, arginfo_test_router_route_setHttpMethods, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router_Route, getHttpMethods, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router_Route, setHostname, arginfo_test_router_route_setHostname, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router_Route, getHostname, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router_Route, convert, arginfo_test_router_route_convert, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router_Route, getConverters, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
