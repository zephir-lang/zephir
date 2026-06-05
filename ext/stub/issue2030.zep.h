
extern zend_class_entry *stub_issue2030_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2030);

PHP_METHOD(Stub_Issue2030, escapeChar);
PHP_METHOD(Stub_Issue2030, unicodeAscii);
PHP_METHOD(Stub_Issue2030, unicode2Byte);
PHP_METHOD(Stub_Issue2030, unicode3Byte);
PHP_METHOD(Stub_Issue2030, unicode4Byte);
PHP_METHOD(Stub_Issue2030, hexFollowedByHex);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2030_escapechar, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2030_unicodeascii, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2030_unicode2byte, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2030_unicode3byte, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2030_unicode4byte, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2030_hexfollowedbyhex, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2030_method_entry) {
	PHP_ME(Stub_Issue2030, escapeChar, arginfo_stub_issue2030_escapechar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2030, unicodeAscii, arginfo_stub_issue2030_unicodeascii, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2030, unicode2Byte, arginfo_stub_issue2030_unicode2byte, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2030, unicode3Byte, arginfo_stub_issue2030_unicode3byte, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2030, unicode4Byte, arginfo_stub_issue2030_unicode4byte, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2030, hexFollowedByHex, arginfo_stub_issue2030_hexfollowedbyhex, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
