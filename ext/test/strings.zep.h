
extern zend_class_entry *test_strings_ce;

ZEPHIR_INIT_CLASS(Test_Strings);

PHP_METHOD(Test_Strings, testTrim);
PHP_METHOD(Test_Strings, testRtrim);
PHP_METHOD(Test_Strings, testLtrim);
PHP_METHOD(Test_Strings, testTrim2Params);
PHP_METHOD(Test_Strings, testRtrim2Params);
PHP_METHOD(Test_Strings, testLtrim2Params);
PHP_METHOD(Test_Strings, testImplode);
PHP_METHOD(Test_Strings, testStrpos);
PHP_METHOD(Test_Strings, testStrposOffset);
PHP_METHOD(Test_Strings, testExplode);
PHP_METHOD(Test_Strings, testExplodeStr);
PHP_METHOD(Test_Strings, testExplodeLimit);
PHP_METHOD(Test_Strings, testSubstr);
PHP_METHOD(Test_Strings, testSubstr2);
PHP_METHOD(Test_Strings, testSubstr3);
PHP_METHOD(Test_Strings, testSubstr4);
PHP_METHOD(Test_Strings, testAddslashes);
PHP_METHOD(Test_Strings, testStripslashes);
PHP_METHOD(Test_Strings, testHardcodedMultilineString);
PHP_METHOD(Test_Strings, testEchoMultilineString);
PHP_METHOD(Test_Strings, testTrimMultilineString);
PHP_METHOD(Test_Strings, testWellEscapedMultilineString);
PHP_METHOD(Test_Strings, testInternedString1);
PHP_METHOD(Test_Strings, testInternedString2);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_strings_testtrim, 0, 0, 1)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_strings_testrtrim, 0, 0, 1)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_strings_testltrim, 0, 0, 1)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_strings_testtrim2params, 0, 0, 2)
	ZEND_ARG_INFO(0, str)
	ZEND_ARG_INFO(0, charlist)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_strings_testrtrim2params, 0, 0, 2)
	ZEND_ARG_INFO(0, str)
	ZEND_ARG_INFO(0, charlist)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_strings_testltrim2params, 0, 0, 2)
	ZEND_ARG_INFO(0, str)
	ZEND_ARG_INFO(0, charlist)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_strings_testimplode, 0, 0, 2)
	ZEND_ARG_INFO(0, glue)
	ZEND_ARG_INFO(0, pieces)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_strings_teststrpos, 0, 0, 2)
	ZEND_ARG_INFO(0, haystack)
	ZEND_ARG_INFO(0, needle)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_strings_teststrposoffset, 0, 0, 3)
	ZEND_ARG_INFO(0, haystack)
	ZEND_ARG_INFO(0, needle)
	ZEND_ARG_INFO(0, offset)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_strings_testexplode, 0, 0, 2)
	ZEND_ARG_INFO(0, delimiter)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_strings_testexplodestr, 0, 0, 1)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_strings_testexplodelimit, 0, 0, 2)
	ZEND_ARG_INFO(0, str)
	ZEND_ARG_INFO(0, limit)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_strings_testsubstr, 0, 0, 3)
	ZEND_ARG_INFO(0, str)
	ZEND_ARG_INFO(0, from)
	ZEND_ARG_INFO(0, len)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_strings_testsubstr2, 0, 0, 2)
	ZEND_ARG_INFO(0, str)
	ZEND_ARG_INFO(0, from)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_strings_testsubstr3, 0, 0, 1)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_strings_testsubstr4, 0, 0, 1)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_strings_testaddslashes, 0, 0, 1)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_strings_teststripslashes, 0, 0, 1)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_strings_method_entry) {
	PHP_ME(Test_Strings, testTrim, arginfo_test_strings_testtrim, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Strings, testRtrim, arginfo_test_strings_testrtrim, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Strings, testLtrim, arginfo_test_strings_testltrim, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Strings, testTrim2Params, arginfo_test_strings_testtrim2params, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Strings, testRtrim2Params, arginfo_test_strings_testrtrim2params, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Strings, testLtrim2Params, arginfo_test_strings_testltrim2params, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Strings, testImplode, arginfo_test_strings_testimplode, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Strings, testStrpos, arginfo_test_strings_teststrpos, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Strings, testStrposOffset, arginfo_test_strings_teststrposoffset, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Strings, testExplode, arginfo_test_strings_testexplode, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Strings, testExplodeStr, arginfo_test_strings_testexplodestr, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Strings, testExplodeLimit, arginfo_test_strings_testexplodelimit, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Strings, testSubstr, arginfo_test_strings_testsubstr, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Strings, testSubstr2, arginfo_test_strings_testsubstr2, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Strings, testSubstr3, arginfo_test_strings_testsubstr3, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Strings, testSubstr4, arginfo_test_strings_testsubstr4, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Strings, testAddslashes, arginfo_test_strings_testaddslashes, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Strings, testStripslashes, arginfo_test_strings_teststripslashes, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Strings, testHardcodedMultilineString, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Strings, testEchoMultilineString, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Strings, testTrimMultilineString, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Strings, testWellEscapedMultilineString, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Strings, testInternedString1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Strings, testInternedString2, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
