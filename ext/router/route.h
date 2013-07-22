
extern zend_class_entry *test_router_route_ce;

ZEPHIR_INIT_CLASS(Test_Router_Route);

PHP_METHOD(Test_Router_Route, __construct);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_router_route___construct, 0, 0, 0)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, paths)
	ZEND_ARG_INFO(0, httpMethods)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_router_route_method_entry) {
	PHP_ME(Test_Router_Route, __construct, arginfo_test_router_route___construct, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
