
extern zend_class_entry *test_strings_ce;

ZEPHIR_INIT_CLASS(Test_Strings);

PHP_METHOD(Test_Strings, testTrim);
PHP_METHOD(Test_Strings, testRtrim);
PHP_METHOD(Test_Strings, testLtrim);
PHP_METHOD(Test_Strings, testImplode);
PHP_METHOD(Test_Strings, testStrpos);
PHP_METHOD(Test_Strings, testStrposOffset);
PHP_METHOD(Test_Strings, testExplode);
PHP_METHOD(Test_Strings, testExplodeStr);
PHP_METHOD(Test_Strings, testExplodeLimit);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_strings_testtrim, 0, 0, 1)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_strings_testrtrim, 0, 0, 1)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_strings_testltrim, 0, 0, 1)
	ZEND_ARG_INFO(0, str)
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

ZEPHIR_INIT_FUNCS(test_strings_method_entry) {
	PHP_ME(Test_Strings, testTrim, arginfo_test_strings_testtrim, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Strings, testRtrim, arginfo_test_strings_testrtrim, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Strings, testLtrim, arginfo_test_strings_testltrim, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Strings, testImplode, arginfo_test_strings_testimplode, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Strings, testStrpos, arginfo_test_strings_teststrpos, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Strings, testStrposOffset, arginfo_test_strings_teststrposoffset, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Strings, testExplode, arginfo_test_strings_testexplode, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Strings, testExplodeStr, arginfo_test_strings_testexplodestr, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Strings, testExplodeLimit, arginfo_test_strings_testexplodelimit, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
