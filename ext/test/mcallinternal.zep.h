
extern zend_class_entry *test_mcallinternal_ce;

ZEPHIR_INIT_CLASS(Test_McallInternal);

void zep_Test_McallInternal_a(int ht, zend_execute_data *execute_data, zval *return_value, zval *this_ptr, int return_value_used);
void zep_Test_McallInternal_b(int ht, zend_execute_data *execute_data, zval *return_value, zval *this_ptr, int return_value_used, zval *a_ext , zval *b_ext );
void zep_Test_McallInternal_c(int ht, zend_execute_data *execute_data, zval *return_value, zval *this_ptr, int return_value_used, zval *a_param_ext, zval *b_param_ext);
PHP_METHOD(Test_McallInternal, e);
PHP_METHOD(Test_McallInternal, d);
PHP_METHOD(Test_McallInternal, f);
PHP_METHOD(Test_McallInternal, g);
void zep_Test_McallInternal_other(int ht, zend_execute_data *execute_data, zval *return_value, zval *this_ptr, int return_value_used, zval *a_param_ext, zval *b_param_ext);
PHP_METHOD(Test_McallInternal, callFibonacci);
void zep_Test_McallInternal_test1956(int ht, zend_execute_data *execute_data, zval *return_value, zval *this_ptr, int return_value_used);
PHP_METHOD(Test_McallInternal, issue1956);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_mcallinternal_b, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_mcallinternal_c, 0, 0, 2)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, a)
#endif
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, b)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_mcallinternal_other, 0, 2, IS_DOUBLE, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_mcallinternal_other, 0, 2, IS_DOUBLE, NULL, 0)
#endif
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, a)
#endif
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, b)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_mcallinternal_callfibonacci, 0, 0, IS_DOUBLE, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_mcallinternal_callfibonacci, 0, 0, IS_DOUBLE, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_test_mcallinternal_test1956, 0, 0, Test\\McallInternal, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_mcallinternal_test1956, 0, 0, IS_OBJECT, "Test\\McallInternal", 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_test_mcallinternal_issue1956, 0, 0, Test\\McallInternal, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_mcallinternal_issue1956, 0, 0, IS_OBJECT, "Test\\McallInternal", 0)
#endif
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_mcallinternal_method_entry) {
	PHP_ME(Test_McallInternal, e, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_McallInternal, d, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_McallInternal, f, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_McallInternal, g, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_McallInternal, callFibonacci, arginfo_test_mcallinternal_callfibonacci, ZEND_ACC_PUBLIC)
	PHP_ME(Test_McallInternal, issue1956, arginfo_test_mcallinternal_issue1956, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
