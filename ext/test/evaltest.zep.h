
extern zend_class_entry *test_evaltest_ce;

ZEPHIR_INIT_CLASS(Test_EvalTest);

PHP_METHOD(Test_EvalTest, evalCode);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_evaltest_evalcode, 0, 0, 1)
	ZEND_ARG_INFO(0, code)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_evaltest_method_entry) {
	PHP_ME(Test_EvalTest, evalCode, arginfo_test_evaltest_evalcode, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
