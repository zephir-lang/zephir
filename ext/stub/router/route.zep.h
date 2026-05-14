
extern zend_class_entry *stub_router_route_ce;

ZEPHIR_INIT_CLASS(Stub_Router_Route);

PHP_METHOD(Stub_Router_Route, __construct);
PHP_METHOD(Stub_Router_Route, compilePattern);
PHP_METHOD(Stub_Router_Route, via);
PHP_METHOD(Stub_Router_Route, extractNamedParams);
PHP_METHOD(Stub_Router_Route, reConfigure);
PHP_METHOD(Stub_Router_Route, getName);
PHP_METHOD(Stub_Router_Route, setName);
PHP_METHOD(Stub_Router_Route, beforeMatch);
PHP_METHOD(Stub_Router_Route, getBeforeMatch);
PHP_METHOD(Stub_Router_Route, getRouteId);
PHP_METHOD(Stub_Router_Route, getPattern);
PHP_METHOD(Stub_Router_Route, getCompiledPattern);
PHP_METHOD(Stub_Router_Route, getPaths);
PHP_METHOD(Stub_Router_Route, getReversedPaths);
PHP_METHOD(Stub_Router_Route, setHttpMethods);
PHP_METHOD(Stub_Router_Route, getHttpMethods);
PHP_METHOD(Stub_Router_Route, setHostname);
PHP_METHOD(Stub_Router_Route, getHostname);
PHP_METHOD(Stub_Router_Route, convert);
PHP_METHOD(Stub_Router_Route, getConverters);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_router_route___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, paths)
	ZEND_ARG_INFO(0, httpMethods)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_router_route_compilepattern, 0, 0, 1)
	ZEND_ARG_INFO(0, pattern)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_router_route_via, 0, 0, 1)
	ZEND_ARG_INFO(0, httpMethods)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_router_route_extractnamedparams, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, pattern, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_router_route_reconfigure, 0, 0, 1)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, paths)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_router_route_getname, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_router_route_setname, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_router_route_beforematch, 0, 0, 1)
	ZEND_ARG_INFO(0, callback)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_router_route_getbeforematch, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_router_route_getrouteid, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_router_route_getpattern, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_router_route_getcompiledpattern, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_router_route_getpaths, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_router_route_getreversedpaths, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_router_route_sethttpmethods, 0, 0, 1)
	ZEND_ARG_INFO(0, httpMethods)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_router_route_gethttpmethods, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_router_route_sethostname, 0, 0, 1)
	ZEND_ARG_INFO(0, hostname)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_router_route_gethostname, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_router_route_convert, 0, 0, 2)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, converter)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_router_route_getconverters, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_router_route_method_entry) {
	PHP_ME(Stub_Router_Route, __construct, arginfo_stub_router_route___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Stub_Router_Route, compilePattern, arginfo_stub_router_route_compilepattern, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Router_Route, via, arginfo_stub_router_route_via, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Router_Route, extractNamedParams, arginfo_stub_router_route_extractnamedparams, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Router_Route, reConfigure, arginfo_stub_router_route_reconfigure, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Router_Route, getName, arginfo_stub_router_route_getname, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Router_Route, setName, arginfo_stub_router_route_setname, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Router_Route, beforeMatch, arginfo_stub_router_route_beforematch, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Router_Route, getBeforeMatch, arginfo_stub_router_route_getbeforematch, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Router_Route, getRouteId, arginfo_stub_router_route_getrouteid, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Router_Route, getPattern, arginfo_stub_router_route_getpattern, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Router_Route, getCompiledPattern, arginfo_stub_router_route_getcompiledpattern, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Router_Route, getPaths, arginfo_stub_router_route_getpaths, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Router_Route, getReversedPaths, arginfo_stub_router_route_getreversedpaths, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Router_Route, setHttpMethods, arginfo_stub_router_route_sethttpmethods, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Router_Route, getHttpMethods, arginfo_stub_router_route_gethttpmethods, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Router_Route, setHostname, arginfo_stub_router_route_sethostname, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Router_Route, getHostname, arginfo_stub_router_route_gethostname, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Router_Route, convert, arginfo_stub_router_route_convert, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Router_Route, getConverters, arginfo_stub_router_route_getconverters, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
