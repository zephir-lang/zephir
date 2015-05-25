
extern zend_class_entry *test_optimizers_substr_ce;

ZEPHIR_INIT_CLASS(Test_Optimizers_Substr);

PHP_METHOD(Test_Optimizers_Substr, testTwoArguments);
PHP_METHOD(Test_Optimizers_Substr, testThreeArguments);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_optimizers_substr_testtwoarguments, 0, 0, 2)
	ZEND_ARG_INFO(0, str)
	ZEND_ARG_INFO(0, start)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_optimizers_substr_testthreearguments, 0, 0, 3)
	ZEND_ARG_INFO(0, str)
	ZEND_ARG_INFO(0, start)
	ZEND_ARG_INFO(0, offset)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_optimizers_substr_method_entry) {
	PHP_ME(Test_Optimizers_Substr, testTwoArguments, arginfo_test_optimizers_substr_testtwoarguments, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Optimizers_Substr, testThreeArguments, arginfo_test_optimizers_substr_testthreearguments, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
