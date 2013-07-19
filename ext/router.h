
extern zend_class_entry *test_router_ce;

ZEPHIR_INIT_CLASS(Test_Router);

PHP_METHOD(Test_Router, __construct);
PHP_METHOD(Test_Router, setDI);
PHP_METHOD(Test_Router, getDI);
PHP_METHOD(Test_Router, add);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_router___construct, 0, 0, 0)
	ZEND_ARG_INFO(0, defaultRoutes)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_router_setDI, 0, 0, 0)
	ZEND_ARG_INFO(0, dependencyInjector)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_router_add, 0, 0, 0)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, paths)
	ZEND_ARG_INFO(0, httpMethods)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_router_method_entry) {
	PHP_ME(Test_Router, __construct, arginfo_test_router___construct, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router, setDI, arginfo_test_router_setDI, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router, getDI, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router, add, arginfo_test_router_add, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
