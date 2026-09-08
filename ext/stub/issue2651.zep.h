
extern zend_class_entry *stub_issue2651_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2651);

PHP_METHOD(Stub_Issue2651, toArray);
PHP_METHOD(Stub_Issue2651, getPreset);
PHP_METHOD(Stub_Issue2651, getNested);
PHP_METHOD(Stub_Issue2651, addHeader);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2651_toarray, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2651_getpreset, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2651_getnested, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2651_addheader, 0, 2, IS_VOID, 0)

	ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, value, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2651_method_entry) {
	PHP_ME(Stub_Issue2651, toArray, arginfo_stub_issue2651_toarray, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2651, getPreset, arginfo_stub_issue2651_getpreset, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2651, getNested, arginfo_stub_issue2651_getnested, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2651, addHeader, arginfo_stub_issue2651_addheader, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
