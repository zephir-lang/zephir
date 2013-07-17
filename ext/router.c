/* hello */

PHP_INIT_CLASS(Router) {

	zend_declare_property_null(router_ce, SL("_dependencyInjector"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(router_ce, SL("_uriSource"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_bool(router_ce, SL("_wasMatched"), 0, ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(router_ce, SL("_wasMatched"), ZEND_ACC_PROTECTED TSRMLS_CC);

}

PHP_METHOD(Router, __construct) {

}

PHP_METHOD(Router, setDI) {

	int a, b, c;
	int d, e, f;
	fprintf(stdout, "%d\n", a);
}

PHP_METHOD(Router, getDI) {

}

