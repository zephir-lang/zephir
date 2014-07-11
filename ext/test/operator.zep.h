
extern zend_class_entry *test_operator_ce;

ZEPHIR_INIT_CLASS(Test_Operator);

PHP_METHOD(Test_Operator, testIdentical);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_operator_testidentical, 0, 0, 2)
	ZEND_ARG_INFO(0, param1)
	ZEND_ARG_INFO(0, param2)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_operator_method_entry) {
	PHP_ME(Test_Operator, testIdentical, arginfo_test_operator_testidentical, ZEND_ACC_PUBLIC)
  PHP_FE_END
};
