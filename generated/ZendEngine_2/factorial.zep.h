
extern zend_class_entry *test_factorial_ce;

ZEPHIR_INIT_CLASS(Test_Factorial);

PHP_METHOD(Test_Factorial, intIterativeFactorial);
PHP_METHOD(Test_Factorial, intRecursiveFactorial);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_factorial_intiterativefactorial, 0, 0, 1)
	ZEND_ARG_INFO(0, n)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_factorial_intrecursivefactorial, 0, 0, 1)
	ZEND_ARG_INFO(0, num)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_factorial_method_entry) {
	PHP_ME(Test_Factorial, intIterativeFactorial, arginfo_test_factorial_intiterativefactorial, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Factorial, intRecursiveFactorial, arginfo_test_factorial_intrecursivefactorial, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
