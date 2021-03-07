
extern zend_class_entry *stub_factorial_ce;

ZEPHIR_INIT_CLASS(Stub_Factorial);

PHP_METHOD(Stub_Factorial, intIterativeFactorial);
PHP_METHOD(Stub_Factorial, intRecursiveFactorial);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_factorial_intiterativefactorial, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_factorial_intrecursivefactorial, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, num, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_factorial_method_entry) {
	PHP_ME(Stub_Factorial, intIterativeFactorial, arginfo_stub_factorial_intiterativefactorial, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Factorial, intRecursiveFactorial, arginfo_stub_factorial_intrecursivefactorial, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
