
extern zend_class_entry *test_pregmatch_ce;

ZEPHIR_INIT_CLASS(Test_Pregmatch);

PHP_METHOD(Test_Pregmatch, testWithoutReturnAndMatches);
PHP_METHOD(Test_Pregmatch, testWithoutReturns);
PHP_METHOD(Test_Pregmatch, testWithoutMatches);

ZEPHIR_INIT_FUNCS(test_pregmatch_method_entry) {
	PHP_ME(Test_Pregmatch, testWithoutReturnAndMatches, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Pregmatch, testWithoutReturns, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Pregmatch, testWithoutMatches, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
