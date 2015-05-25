
extern zend_class_entry *test_concat_ce;

ZEPHIR_INIT_CLASS(Test_Concat);

PHP_METHOD(Test_Concat, getTestProperty);
PHP_METHOD(Test_Concat, testConcatBySelfProperty);
PHP_METHOD(Test_Concat, testConcat1);
PHP_METHOD(Test_Concat, testConcat2);
PHP_METHOD(Test_Concat, testConcatSelf1);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_concat_testconcatbyselfproperty, 0, 0, 1)
	ZEND_ARG_INFO(0, title)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_concat_method_entry) {
	PHP_ME(Test_Concat, getTestProperty, NULL, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Test_Concat, testConcatBySelfProperty, arginfo_test_concat_testconcatbyselfproperty, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Test_Concat, testConcat1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Concat, testConcat2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Concat, testConcatSelf1, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
