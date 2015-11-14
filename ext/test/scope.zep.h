
extern zend_class_entry *test_scope_ce;

ZEPHIR_INIT_CLASS(Test_Scope);

PHP_METHOD(Test_Scope, getStr);
PHP_METHOD(Test_Scope, getDyStr);
PHP_METHOD(Test_Scope, test1);
PHP_METHOD(Test_Scope, test2);
PHP_METHOD(Test_Scope, test3);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_scope_getdystr, 0, 0, 1)
	ZEND_ARG_INFO(0, g)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_scope_method_entry) {
	PHP_ME(Test_Scope, getStr, NULL, ZEND_ACC_PRIVATE|ZEND_ACC_STATIC)
	PHP_ME(Test_Scope, getDyStr, arginfo_test_scope_getdystr, ZEND_ACC_PRIVATE|ZEND_ACC_STATIC)
	PHP_ME(Test_Scope, test1, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Test_Scope, test2, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Test_Scope, test3, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
