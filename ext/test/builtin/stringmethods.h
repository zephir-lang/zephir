
extern zend_class_entry *test_builtin_stringmethods_ce;

ZEPHIR_INIT_CLASS(Test_BuiltIn_StringMethods);

PHP_METHOD(Test_BuiltIn_StringMethods, getLength1);
PHP_METHOD(Test_BuiltIn_StringMethods, getLength2);
PHP_METHOD(Test_BuiltIn_StringMethods, getLength3);
PHP_METHOD(Test_BuiltIn_StringMethods, getLength4);
PHP_METHOD(Test_BuiltIn_StringMethods, getLength5);
PHP_METHOD(Test_BuiltIn_StringMethods, getIndex);
PHP_METHOD(Test_BuiltIn_StringMethods, getIndexWithPosition);
PHP_METHOD(Test_BuiltIn_StringMethods, getTrimmed);
PHP_METHOD(Test_BuiltIn_StringMethods, getTrimmed1);
PHP_METHOD(Test_BuiltIn_StringMethods, getLeftTrimmed);
PHP_METHOD(Test_BuiltIn_StringMethods, getRightTrimmed);
PHP_METHOD(Test_BuiltIn_StringMethods, getLower);
PHP_METHOD(Test_BuiltIn_StringMethods, getUpper);
PHP_METHOD(Test_BuiltIn_StringMethods, getLowerFirst);
PHP_METHOD(Test_BuiltIn_StringMethods, getUpperFirst);
PHP_METHOD(Test_BuiltIn_StringMethods, getFormatted);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_builtin_stringmethods_getlength4, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_builtin_stringmethods_getlength5, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_builtin_stringmethods_getindex, 0, 0, 2)
	ZEND_ARG_INFO(0, str)
	ZEND_ARG_INFO(0, needle)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_builtin_stringmethods_getindexwithposition, 0, 0, 3)
	ZEND_ARG_INFO(0, str)
	ZEND_ARG_INFO(0, needle)
	ZEND_ARG_INFO(0, position)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_builtin_stringmethods_gettrimmed1, 0, 0, 1)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_builtin_stringmethods_getlefttrimmed, 0, 0, 1)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_builtin_stringmethods_getrighttrimmed, 0, 0, 1)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_builtin_stringmethods_getlower, 0, 0, 1)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_builtin_stringmethods_getupper, 0, 0, 1)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_builtin_stringmethods_getlowerfirst, 0, 0, 1)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_builtin_stringmethods_getupperfirst, 0, 0, 1)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_builtin_stringmethods_getformatted, 0, 0, 1)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_builtin_stringmethods_method_entry) {
	PHP_ME(Test_BuiltIn_StringMethods, getLength1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_BuiltIn_StringMethods, getLength2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_BuiltIn_StringMethods, getLength3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_BuiltIn_StringMethods, getLength4, arginfo_test_builtin_stringmethods_getlength4, ZEND_ACC_PUBLIC)
	PHP_ME(Test_BuiltIn_StringMethods, getLength5, arginfo_test_builtin_stringmethods_getlength5, ZEND_ACC_PUBLIC)
	PHP_ME(Test_BuiltIn_StringMethods, getIndex, arginfo_test_builtin_stringmethods_getindex, ZEND_ACC_PUBLIC)
	PHP_ME(Test_BuiltIn_StringMethods, getIndexWithPosition, arginfo_test_builtin_stringmethods_getindexwithposition, ZEND_ACC_PUBLIC)
	PHP_ME(Test_BuiltIn_StringMethods, getTrimmed, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_BuiltIn_StringMethods, getTrimmed1, arginfo_test_builtin_stringmethods_gettrimmed1, ZEND_ACC_PUBLIC)
	PHP_ME(Test_BuiltIn_StringMethods, getLeftTrimmed, arginfo_test_builtin_stringmethods_getlefttrimmed, ZEND_ACC_PUBLIC)
	PHP_ME(Test_BuiltIn_StringMethods, getRightTrimmed, arginfo_test_builtin_stringmethods_getrighttrimmed, ZEND_ACC_PUBLIC)
	PHP_ME(Test_BuiltIn_StringMethods, getLower, arginfo_test_builtin_stringmethods_getlower, ZEND_ACC_PUBLIC)
	PHP_ME(Test_BuiltIn_StringMethods, getUpper, arginfo_test_builtin_stringmethods_getupper, ZEND_ACC_PUBLIC)
	PHP_ME(Test_BuiltIn_StringMethods, getLowerFirst, arginfo_test_builtin_stringmethods_getlowerfirst, ZEND_ACC_PUBLIC)
	PHP_ME(Test_BuiltIn_StringMethods, getUpperFirst, arginfo_test_builtin_stringmethods_getupperfirst, ZEND_ACC_PUBLIC)
	PHP_ME(Test_BuiltIn_StringMethods, getFormatted, arginfo_test_builtin_stringmethods_getformatted, ZEND_ACC_PUBLIC)
  PHP_FE_END
};
