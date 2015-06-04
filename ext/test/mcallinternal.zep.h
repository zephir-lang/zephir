
extern zend_class_entry *test_mcallinternal_ce;

ZEPHIR_INIT_CLASS(Test_McallInternal);

static void zep_Test_McallInternal_a(int ht, zval *return_value, zval **return_value_ptr, zval *this_ptr, int return_value_used TSRMLS_DC);
static void zep_Test_McallInternal_b(int ht, zval *return_value, zval **return_value_ptr, zval *this_ptr, int return_value_used, zval *a_ext, zval *b_ext TSRMLS_DC);
static void zep_Test_McallInternal_c(int ht, zval *return_value, zval **return_value_ptr, zval *this_ptr, int return_value_used, zval *a_param_ext, zval *b_param_ext TSRMLS_DC);
PHP_METHOD(Test_McallInternal, e);
PHP_METHOD(Test_McallInternal, d);
PHP_METHOD(Test_McallInternal, f);
PHP_METHOD(Test_McallInternal, g);
static void zep_Test_McallInternal_fibonacci(int ht, zval *return_value, zval **return_value_ptr, zval *this_ptr, int return_value_used, zval *n_param_ext TSRMLS_DC);
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
