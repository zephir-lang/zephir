
extern zend_class_entry *test_mcallinternal_ce;

ZEPHIR_INIT_CLASS(Test_McallInternal);

PHP_METHOD(Test_McallInternal, a);
PHP_METHOD(Test_McallInternal, b);
PHP_METHOD(Test_McallInternal, c);
PHP_METHOD(Test_McallInternal, e);
PHP_METHOD(Test_McallInternal, d);
PHP_METHOD(Test_McallInternal, f);
PHP_METHOD(Test_McallInternal, g);
PHP_METHOD(Test_McallInternal, fibonacci);
PHP_METHOD(Test_McallInternal, callFibonacci);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_mcallinternal_b, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_mcallinternal_c, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_mcallinternal_fibonacci, 0, 0, 1)
	ZEND_ARG_INFO(0, n)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_mcallinternal_callfibonacci, 0, 0, 1)
	ZEND_ARG_INFO(0, n)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_mcallinternal_method_entry) {
	PHP_ME(Test_McallInternal, e, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_McallInternal, d, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_McallInternal, f, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_McallInternal, g, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_McallInternal, callFibonacci, arginfo_test_mcallinternal_callfibonacci, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
