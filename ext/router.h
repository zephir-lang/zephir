
extern zend_class_entry *test_router_ce;

TEST_INIT_CLASS(Test_Router);

PHP_METHOD(Test_Router, __construct);
PHP_METHOD(Test_Router, setDI);
PHP_METHOD(Test_Router, getDI);

TEST_INIT_FUNCS(test_router_method_entry) {
	PHP_ME(Test_Router, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Test_Router, setDI, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Router, getDI, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
