
extern zend_class_entry *test_operator_ce;

ZEPHIR_INIT_CLASS(Test_Operator);

PHP_METHOD(Test_Operator, testIdentical);
PHP_METHOD(Test_Operator, testExponential);
PHP_METHOD(Test_Operator, testExponential2);
PHP_METHOD(Test_Operator, testExponential3);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_operator_testidentical, 0, 0, 2)
	ZEND_ARG_INFO(0, param1)
	ZEND_ARG_INFO(0, param2)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_operator_testexponential, 0, 0, 3)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
	ZEND_ARG_INFO(0, c)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_operator_testexponential3, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_operator_method_entry) {
	PHP_ME(Test_Operator, testIdentical, arginfo_test_operator_testidentical, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Operator, testExponential, arginfo_test_operator_testexponential, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Operator, testExponential2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Operator, testExponential3, arginfo_test_operator_testexponential3, ZEND_ACC_PUBLIC)
  PHP_FE_END
};
