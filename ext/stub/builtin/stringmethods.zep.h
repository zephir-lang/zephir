
extern zend_class_entry *stub_builtin_stringmethods_ce;

ZEPHIR_INIT_CLASS(Stub_BuiltIn_StringMethods);

PHP_METHOD(Stub_BuiltIn_StringMethods, camelize);
PHP_METHOD(Stub_BuiltIn_StringMethods, uncamelize);
PHP_METHOD(Stub_BuiltIn_StringMethods, getLength1);
PHP_METHOD(Stub_BuiltIn_StringMethods, getLength2);
PHP_METHOD(Stub_BuiltIn_StringMethods, getLength3);
PHP_METHOD(Stub_BuiltIn_StringMethods, getLength4);
PHP_METHOD(Stub_BuiltIn_StringMethods, getLength5);
PHP_METHOD(Stub_BuiltIn_StringMethods, getIndex);
PHP_METHOD(Stub_BuiltIn_StringMethods, getIndexWithPosition);
PHP_METHOD(Stub_BuiltIn_StringMethods, getTrimmed);
PHP_METHOD(Stub_BuiltIn_StringMethods, getTrimmed1);
PHP_METHOD(Stub_BuiltIn_StringMethods, getLeftTrimmed);
PHP_METHOD(Stub_BuiltIn_StringMethods, getRightTrimmed);
PHP_METHOD(Stub_BuiltIn_StringMethods, getLower);
PHP_METHOD(Stub_BuiltIn_StringMethods, getUpper);
PHP_METHOD(Stub_BuiltIn_StringMethods, getLowerFirst);
PHP_METHOD(Stub_BuiltIn_StringMethods, getUpperFirst);
PHP_METHOD(Stub_BuiltIn_StringMethods, getFormatted);
PHP_METHOD(Stub_BuiltIn_StringMethods, getMd5);
PHP_METHOD(Stub_BuiltIn_StringMethods, getSha1);
PHP_METHOD(Stub_BuiltIn_StringMethods, getNl2br);
PHP_METHOD(Stub_BuiltIn_StringMethods, getParsedCsv);
PHP_METHOD(Stub_BuiltIn_StringMethods, getParsedJson);
PHP_METHOD(Stub_BuiltIn_StringMethods, getRepeatted);
PHP_METHOD(Stub_BuiltIn_StringMethods, getShuffled);
PHP_METHOD(Stub_BuiltIn_StringMethods, getSplited);
PHP_METHOD(Stub_BuiltIn_StringMethods, getCompare);
PHP_METHOD(Stub_BuiltIn_StringMethods, getCompareLocale);
PHP_METHOD(Stub_BuiltIn_StringMethods, getReversed);
PHP_METHOD(Stub_BuiltIn_StringMethods, getHtmlSpecialChars);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_builtin_stringmethods_camelize, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
	ZEND_ARG_INFO(0, delimiter)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_builtin_stringmethods_uncamelize, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
	ZEND_ARG_INFO(0, delimiter)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_stringmethods_getlength1, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_stringmethods_getlength2, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_stringmethods_getlength3, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_stringmethods_getlength4, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, a, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_stringmethods_getlength5, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, a, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_stringmethods_getindex, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, needle, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_stringmethods_getindexwithposition, 0, 0, 3)
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, needle, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, position, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_stringmethods_gettrimmed, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_stringmethods_gettrimmed1, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_stringmethods_getlefttrimmed, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_stringmethods_getrighttrimmed, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_stringmethods_getlower, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_stringmethods_getupper, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_stringmethods_getlowerfirst, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_stringmethods_getupperfirst, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_stringmethods_getformatted, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_stringmethods_getmd5, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_stringmethods_getsha1, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_stringmethods_getnl2br, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_stringmethods_getparsedcsv, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_stringmethods_getparsedjson, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, assoc, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_stringmethods_getrepeatted, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, count, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_stringmethods_getshuffled, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_stringmethods_getsplited, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, del, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_stringmethods_getcompare, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, left, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, right, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_stringmethods_getcomparelocale, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, left, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, right, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_stringmethods_getreversed, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_stringmethods_gethtmlspecialchars, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_builtin_stringmethods_method_entry) {
	PHP_ME(Stub_BuiltIn_StringMethods, camelize, arginfo_stub_builtin_stringmethods_camelize, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BuiltIn_StringMethods, uncamelize, arginfo_stub_builtin_stringmethods_uncamelize, ZEND_ACC_PUBLIC)
PHP_ME(Stub_BuiltIn_StringMethods, getLength1, arginfo_stub_builtin_stringmethods_getlength1, ZEND_ACC_PUBLIC)
PHP_ME(Stub_BuiltIn_StringMethods, getLength2, arginfo_stub_builtin_stringmethods_getlength2, ZEND_ACC_PUBLIC)
PHP_ME(Stub_BuiltIn_StringMethods, getLength3, arginfo_stub_builtin_stringmethods_getlength3, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BuiltIn_StringMethods, getLength4, arginfo_stub_builtin_stringmethods_getlength4, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BuiltIn_StringMethods, getLength5, arginfo_stub_builtin_stringmethods_getlength5, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BuiltIn_StringMethods, getIndex, arginfo_stub_builtin_stringmethods_getindex, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BuiltIn_StringMethods, getIndexWithPosition, arginfo_stub_builtin_stringmethods_getindexwithposition, ZEND_ACC_PUBLIC)
PHP_ME(Stub_BuiltIn_StringMethods, getTrimmed, arginfo_stub_builtin_stringmethods_gettrimmed, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BuiltIn_StringMethods, getTrimmed1, arginfo_stub_builtin_stringmethods_gettrimmed1, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BuiltIn_StringMethods, getLeftTrimmed, arginfo_stub_builtin_stringmethods_getlefttrimmed, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BuiltIn_StringMethods, getRightTrimmed, arginfo_stub_builtin_stringmethods_getrighttrimmed, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BuiltIn_StringMethods, getLower, arginfo_stub_builtin_stringmethods_getlower, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BuiltIn_StringMethods, getUpper, arginfo_stub_builtin_stringmethods_getupper, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BuiltIn_StringMethods, getLowerFirst, arginfo_stub_builtin_stringmethods_getlowerfirst, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BuiltIn_StringMethods, getUpperFirst, arginfo_stub_builtin_stringmethods_getupperfirst, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BuiltIn_StringMethods, getFormatted, arginfo_stub_builtin_stringmethods_getformatted, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BuiltIn_StringMethods, getMd5, arginfo_stub_builtin_stringmethods_getmd5, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BuiltIn_StringMethods, getSha1, arginfo_stub_builtin_stringmethods_getsha1, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BuiltIn_StringMethods, getNl2br, arginfo_stub_builtin_stringmethods_getnl2br, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BuiltIn_StringMethods, getParsedCsv, arginfo_stub_builtin_stringmethods_getparsedcsv, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BuiltIn_StringMethods, getParsedJson, arginfo_stub_builtin_stringmethods_getparsedjson, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BuiltIn_StringMethods, getRepeatted, arginfo_stub_builtin_stringmethods_getrepeatted, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BuiltIn_StringMethods, getShuffled, arginfo_stub_builtin_stringmethods_getshuffled, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BuiltIn_StringMethods, getSplited, arginfo_stub_builtin_stringmethods_getsplited, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BuiltIn_StringMethods, getCompare, arginfo_stub_builtin_stringmethods_getcompare, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BuiltIn_StringMethods, getCompareLocale, arginfo_stub_builtin_stringmethods_getcomparelocale, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BuiltIn_StringMethods, getReversed, arginfo_stub_builtin_stringmethods_getreversed, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BuiltIn_StringMethods, getHtmlSpecialChars, arginfo_stub_builtin_stringmethods_gethtmlspecialchars, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
