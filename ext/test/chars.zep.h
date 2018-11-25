
extern zend_class_entry *test_chars_ce;

ZEPHIR_INIT_CLASS(Test_Chars);

PHP_METHOD(Test_Chars, sumChars1);
PHP_METHOD(Test_Chars, sumChars2);
PHP_METHOD(Test_Chars, diffChars1);
PHP_METHOD(Test_Chars, diffChars2);

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_chars_sumchars1, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_chars_sumchars1, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_chars_sumchars2, 0, 1, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_chars_sumchars2, 0, 1, IS_LONG, NULL, 0)
#endif
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, ch, IS_STRING, 0)
#else
	ZEND_ARG_INFO(0, ch)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_chars_diffchars1, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_chars_diffchars1, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_chars_diffchars2, 0, 1, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_chars_diffchars2, 0, 1, IS_LONG, NULL, 0)
#endif
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, ch, IS_STRING, 0)
#else
	ZEND_ARG_INFO(0, ch)
#endif
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_chars_method_entry) {
	PHP_ME(Test_Chars, sumChars1, arginfo_test_chars_sumchars1, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Chars, sumChars2, arginfo_test_chars_sumchars2, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Chars, diffChars1, arginfo_test_chars_diffchars1, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Chars, diffChars2, arginfo_test_chars_diffchars2, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
