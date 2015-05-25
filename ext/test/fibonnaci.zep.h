
extern zend_class_entry *test_fibonnaci_ce;

ZEPHIR_INIT_CLASS(Test_Fibonnaci);

PHP_METHOD(Test_Fibonnaci, fibInt);
PHP_METHOD(Test_Fibonnaci, fibDouble);
PHP_METHOD(Test_Fibonnaci, fibArray);
PHP_METHOD(Test_Fibonnaci, fibArray2);
PHP_METHOD(Test_Fibonnaci, fibonacciRecursive);
PHP_METHOD(Test_Fibonnaci, fibonacciFinalRecursive);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_fibonnaci_fibonaccirecursive, 0, 0, 1)
	ZEND_ARG_INFO(0, n)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_fibonnaci_fibonaccifinalrecursive, 0, 0, 1)
	ZEND_ARG_INFO(0, n)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_fibonnaci_method_entry) {
	PHP_ME(Test_Fibonnaci, fibInt, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Fibonnaci, fibDouble, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Fibonnaci, fibArray, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Fibonnaci, fibArray2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Fibonnaci, fibonacciRecursive, arginfo_test_fibonnaci_fibonaccirecursive, ZEND_ACC_PRIVATE)
	PHP_ME(Test_Fibonnaci, fibonacciFinalRecursive, arginfo_test_fibonnaci_fibonaccifinalrecursive, ZEND_ACC_PRIVATE|ZEND_ACC_FINAL)
	PHP_FE_END
};
