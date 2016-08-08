
extern zend_class_entry *test_chars_ce;

ZEPHIR_INIT_CLASS(Test_Chars);

PHP_METHOD(Test_Chars, sumChars1);
PHP_METHOD(Test_Chars, sumChars2);
PHP_METHOD(Test_Chars, diffChars1);
PHP_METHOD(Test_Chars, diffChars2);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_chars_sumchars2, 0, 0, 1)
	ZEND_ARG_INFO(0, ch)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_chars_diffchars2, 0, 0, 1)
	ZEND_ARG_INFO(0, ch)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_chars_method_entry) {
	PHP_ME(Test_Chars, sumChars1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Chars, sumChars2, arginfo_test_chars_sumchars2, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Chars, diffChars1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Chars, diffChars2, arginfo_test_chars_diffchars2, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
