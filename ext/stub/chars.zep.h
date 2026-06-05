
extern zend_class_entry *stub_chars_ce;

ZEPHIR_INIT_CLASS(Stub_Chars);

PHP_METHOD(Stub_Chars, sumChars1);
PHP_METHOD(Stub_Chars, sumChars2);
PHP_METHOD(Stub_Chars, diffChars1);
PHP_METHOD(Stub_Chars, diffChars2);
PHP_METHOD(Stub_Chars, sumOverflowHex);
PHP_METHOD(Stub_Chars, sumOverflowValue);
PHP_METHOD(Stub_Chars, sumCharParamOverflow);
PHP_METHOD(Stub_Chars, charUnderflow);
PHP_METHOD(Stub_Chars, maxByteValue);
PHP_METHOD(Stub_Chars, sumUchar);
PHP_METHOD(Stub_Chars, arrayOfChars);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_chars_sumchars1, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_chars_sumchars2, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, ch, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_chars_diffchars1, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_chars_diffchars2, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, ch, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_chars_sumoverflowhex, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_chars_sumoverflowvalue, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_chars_sumcharparamoverflow, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, ch, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_chars_charunderflow, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_chars_maxbytevalue, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_chars_sumuchar, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_chars_arrayofchars, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_chars_method_entry) {
	PHP_ME(Stub_Chars, sumChars1, arginfo_stub_chars_sumchars1, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Chars, sumChars2, arginfo_stub_chars_sumchars2, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Chars, diffChars1, arginfo_stub_chars_diffchars1, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Chars, diffChars2, arginfo_stub_chars_diffchars2, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Chars, sumOverflowHex, arginfo_stub_chars_sumoverflowhex, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Chars, sumOverflowValue, arginfo_stub_chars_sumoverflowvalue, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Chars, sumCharParamOverflow, arginfo_stub_chars_sumcharparamoverflow, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Chars, charUnderflow, arginfo_stub_chars_charunderflow, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Chars, maxByteValue, arginfo_stub_chars_maxbytevalue, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Chars, sumUchar, arginfo_stub_chars_sumuchar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Chars, arrayOfChars, arginfo_stub_chars_arrayofchars, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
