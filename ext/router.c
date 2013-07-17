/* hello */

TEST_INIT_CLASS(Router) {

	TEST_REGISTER_CLASS(Test, Router, router, router_method_entry, 0);

	zend_declare_property_null(router_ce, SL("_dependencyInjector"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(router_ce, SL("_uriSource"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_bool(router_ce, SL("_wasMatched"), 0, ZEND_ACC_PROTECTED TSRMLS_CC);

}

PHP_METHOD(Router, __construct) {

}

PHP_METHOD(Router, setDI) {

	int a, b, c;
	double d, e, f;
	fprintf(stdout, "%d", a);
}

PHP_METHOD(Router, getDI) {

}

