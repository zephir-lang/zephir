
extern zend_class_entry *stub_strings_ce;

ZEPHIR_INIT_CLASS(Stub_Strings);

PHP_METHOD(Stub_Strings, camelize);
PHP_METHOD(Stub_Strings, uncamelize);
PHP_METHOD(Stub_Strings, testTrim);
PHP_METHOD(Stub_Strings, testRtrim);
PHP_METHOD(Stub_Strings, testLtrim);
PHP_METHOD(Stub_Strings, testTrim2Params);
PHP_METHOD(Stub_Strings, testRtrim2Params);
PHP_METHOD(Stub_Strings, testLtrim2Params);
PHP_METHOD(Stub_Strings, testImplode);
PHP_METHOD(Stub_Strings, testStrpos);
PHP_METHOD(Stub_Strings, testStrposOffset);
PHP_METHOD(Stub_Strings, testExplode);
PHP_METHOD(Stub_Strings, testExplodeStr);
PHP_METHOD(Stub_Strings, testExplodeLimit);
PHP_METHOD(Stub_Strings, testSubstr);
PHP_METHOD(Stub_Strings, testSubstr2);
PHP_METHOD(Stub_Strings, testSubstr3);
PHP_METHOD(Stub_Strings, testSubstr4);
PHP_METHOD(Stub_Strings, testAddslashes);
PHP_METHOD(Stub_Strings, testStripslashes);
PHP_METHOD(Stub_Strings, testStripcslashes);
PHP_METHOD(Stub_Strings, testHashEquals);
PHP_METHOD(Stub_Strings, testHardcodedMultilineString);
PHP_METHOD(Stub_Strings, testEchoMultilineString);
PHP_METHOD(Stub_Strings, testTrimMultilineString);
PHP_METHOD(Stub_Strings, testWellEscapedMultilineString);
PHP_METHOD(Stub_Strings, testInternedString1);
PHP_METHOD(Stub_Strings, testInternedString2);
PHP_METHOD(Stub_Strings, strToHex);
PHP_METHOD(Stub_Strings, issue1267);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_strings_camelize, 0, 0, 1)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
#else
	ZEND_ARG_INFO(0, str)
#endif
	ZEND_ARG_INFO(0, delimiter)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_strings_uncamelize, 0, 0, 1)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
#else
	ZEND_ARG_INFO(0, str)
#endif
	ZEND_ARG_INFO(0, delimiter)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_strings_testtrim, 0, 0, 1)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_strings_testrtrim, 0, 0, 1)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_strings_testltrim, 0, 0, 1)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_strings_testtrim2params, 0, 0, 2)
	ZEND_ARG_INFO(0, str)
	ZEND_ARG_INFO(0, charlist)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_strings_testrtrim2params, 0, 0, 2)
	ZEND_ARG_INFO(0, str)
	ZEND_ARG_INFO(0, charlist)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_strings_testltrim2params, 0, 0, 2)
	ZEND_ARG_INFO(0, str)
	ZEND_ARG_INFO(0, charlist)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_strings_testimplode, 0, 0, 2)
	ZEND_ARG_INFO(0, glue)
	ZEND_ARG_INFO(0, pieces)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_strings_teststrpos, 0, 0, 2)
	ZEND_ARG_INFO(0, haystack)
	ZEND_ARG_INFO(0, needle)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_strings_teststrposoffset, 0, 0, 3)
	ZEND_ARG_INFO(0, haystack)
	ZEND_ARG_INFO(0, needle)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, offset, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, offset)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_strings_testexplode, 0, 0, 2)
	ZEND_ARG_INFO(0, delimiter)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_strings_testexplodestr, 0, 0, 1)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_strings_testexplodelimit, 0, 0, 2)
	ZEND_ARG_INFO(0, str)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, limit, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, limit)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_strings_testsubstr, 0, 0, 3)
	ZEND_ARG_INFO(0, str)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, from, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, from)
#endif
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, len, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, len)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_strings_testsubstr2, 0, 0, 2)
	ZEND_ARG_INFO(0, str)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, from, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, from)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_strings_testsubstr3, 0, 0, 1)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_strings_testsubstr4, 0, 0, 1)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_strings_testaddslashes, 0, 0, 1)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_strings_teststripslashes, 0, 0, 1)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_strings_teststripcslashes, 0, 1, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_strings_teststripcslashes, 0, 1, IS_STRING, NULL, 0)
#endif
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_strings_testhashequals, 0, 2, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_strings_testhashequals, 0, 2, _IS_BOOL, NULL, 0)
#endif
	ZEND_ARG_INFO(0, str1)
	ZEND_ARG_INFO(0, str2)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_strings_strtohex, 0, 1, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_strings_strtohex, 0, 1, IS_STRING, NULL, 0)
#endif
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, value, IS_STRING, 0)
#else
	ZEND_ARG_INFO(0, value)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_strings_issue1267, 0, 0, 1)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_strings_method_entry) {
	PHP_ME(Stub_Strings, camelize, arginfo_stub_strings_camelize, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Strings, uncamelize, arginfo_stub_strings_uncamelize, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Strings, testTrim, arginfo_stub_strings_testtrim, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Strings, testRtrim, arginfo_stub_strings_testrtrim, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Strings, testLtrim, arginfo_stub_strings_testltrim, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Strings, testTrim2Params, arginfo_stub_strings_testtrim2params, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Strings, testRtrim2Params, arginfo_stub_strings_testrtrim2params, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Strings, testLtrim2Params, arginfo_stub_strings_testltrim2params, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Strings, testImplode, arginfo_stub_strings_testimplode, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Strings, testStrpos, arginfo_stub_strings_teststrpos, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Strings, testStrposOffset, arginfo_stub_strings_teststrposoffset, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Strings, testExplode, arginfo_stub_strings_testexplode, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Strings, testExplodeStr, arginfo_stub_strings_testexplodestr, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Strings, testExplodeLimit, arginfo_stub_strings_testexplodelimit, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Strings, testSubstr, arginfo_stub_strings_testsubstr, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Strings, testSubstr2, arginfo_stub_strings_testsubstr2, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Strings, testSubstr3, arginfo_stub_strings_testsubstr3, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Strings, testSubstr4, arginfo_stub_strings_testsubstr4, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Strings, testAddslashes, arginfo_stub_strings_testaddslashes, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Strings, testStripslashes, arginfo_stub_strings_teststripslashes, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Strings, testStripcslashes, arginfo_stub_strings_teststripcslashes, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Strings, testHashEquals, arginfo_stub_strings_testhashequals, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Strings, testHardcodedMultilineString, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Strings, testEchoMultilineString, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Strings, testTrimMultilineString, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Strings, testWellEscapedMultilineString, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Strings, testInternedString1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Strings, testInternedString2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Strings, strToHex, arginfo_stub_strings_strtohex, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Strings, issue1267, arginfo_stub_strings_issue1267, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
