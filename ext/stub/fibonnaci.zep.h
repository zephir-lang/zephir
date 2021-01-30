
extern zend_class_entry *stub_fibonnaci_ce;

ZEPHIR_INIT_CLASS(Stub_Fibonnaci);

PHP_METHOD(Stub_Fibonnaci, fibInt);
PHP_METHOD(Stub_Fibonnaci, fibDouble);
PHP_METHOD(Stub_Fibonnaci, fibArray);
PHP_METHOD(Stub_Fibonnaci, fibArray2);
PHP_METHOD(Stub_Fibonnaci, fibonacciRecursive);
PHP_METHOD(Stub_Fibonnaci, fibonacciFinalRecursive);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_fibonnaci_fibint, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_fibonnaci_fibdouble, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_fibonnaci_fibarray, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_fibonnaci_fibarray2, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_fibonnaci_fibonaccirecursive, 0, 0, 1)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, n)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_fibonnaci_fibonaccifinalrecursive, 0, 0, 1)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, n)
#endif
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_fibonnaci_method_entry) {
	PHP_ME(Stub_Fibonnaci, fibInt, arginfo_stub_fibonnaci_fibint, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Fibonnaci, fibDouble, arginfo_stub_fibonnaci_fibdouble, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Fibonnaci, fibArray, arginfo_stub_fibonnaci_fibarray, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Fibonnaci, fibArray2, arginfo_stub_fibonnaci_fibarray2, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Fibonnaci, fibonacciRecursive, arginfo_stub_fibonnaci_fibonaccirecursive, ZEND_ACC_PRIVATE)
	PHP_ME(Stub_Fibonnaci, fibonacciFinalRecursive, arginfo_stub_fibonnaci_fibonaccifinalrecursive, ZEND_ACC_PRIVATE|ZEND_ACC_FINAL)
	PHP_FE_END
};
