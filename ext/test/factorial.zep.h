
extern zend_class_entry *test_factorial_ce;

ZEPHIR_INIT_CLASS(Test_Factorial);

PHP_METHOD(Test_Factorial, intIterativeFactorial);
PHP_METHOD(Test_Factorial, intRecursiveFactorial);

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_factorial_intiterativefactorial, 0, 1, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_factorial_intiterativefactorial, 0, 1, IS_LONG, NULL, 0)
#endif
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, n)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_factorial_intrecursivefactorial, 0, 1, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_factorial_intrecursivefactorial, 0, 1, IS_LONG, NULL, 0)
#endif
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, num, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, num)
#endif
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_factorial_method_entry) {
	PHP_ME(Test_Factorial, intIterativeFactorial, arginfo_test_factorial_intiterativefactorial, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Factorial, intRecursiveFactorial, arginfo_test_factorial_intrecursivefactorial, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
