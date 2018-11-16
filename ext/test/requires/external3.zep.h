
extern zend_class_entry *test_requires_external3_ce;

ZEPHIR_INIT_CLASS(Test_Requires_External3);

PHP_METHOD(Test_Requires_External3, req);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_requires_external3_req, 0, 0, 2)
	ZEND_ARG_INFO(0, path)
	ZEND_ARG_INFO(0, requires)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_requires_external3_method_entry) {
	PHP_ME(Test_Requires_External3, req, arginfo_test_requires_external3_req, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
