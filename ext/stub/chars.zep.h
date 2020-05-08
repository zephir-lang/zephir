
extern zend_class_entry *stub_chars_ce;

ZEPHIR_INIT_CLASS(Stub_Chars);

PHP_METHOD(Stub_Chars, sumChars1);
PHP_METHOD(Stub_Chars, sumChars2);
PHP_METHOD(Stub_Chars, diffChars1);
PHP_METHOD(Stub_Chars, diffChars2);

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_chars_sumchars1, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_chars_sumchars1, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_chars_sumchars2, 0, 1, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_chars_sumchars2, 0, 1, IS_LONG, NULL, 0)
#endif
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, ch, IS_STRING, 0)
#else
	ZEND_ARG_INFO(0, ch)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_chars_diffchars1, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_chars_diffchars1, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_chars_diffchars2, 0, 1, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_chars_diffchars2, 0, 1, IS_LONG, NULL, 0)
#endif
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, ch, IS_STRING, 0)
#else
	ZEND_ARG_INFO(0, ch)
#endif
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_chars_method_entry) {
	PHP_ME(Stub_Chars, sumChars1, arginfo_stub_chars_sumchars1, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Chars, sumChars2, arginfo_stub_chars_sumchars2, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Chars, diffChars1, arginfo_stub_chars_diffchars1, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Chars, diffChars2, arginfo_stub_chars_diffchars2, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
