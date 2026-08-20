
extern zend_class_entry *stub_issue2061_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2061);

PHP_METHOD(Stub_Issue2061, getSize);
PHP_METHOD(Stub_Issue2061, getMask);
PHP_METHOD(Stub_Issue2061, getTotal);
PHP_METHOD(Stub_Issue2061, getLabel);
PHP_METHOD(Stub_Issue2061, withDefault);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2061_getsize, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2061_getmask, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2061_gettotal, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2061_getlabel, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2061_withdefault, 0, 0, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, size, IS_LONG, 0, "2048")
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2061_method_entry) {
PHP_ME(Stub_Issue2061, getSize, arginfo_stub_issue2061_getsize, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Issue2061, getMask, arginfo_stub_issue2061_getmask, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Issue2061, getTotal, arginfo_stub_issue2061_gettotal, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Issue2061, getLabel, arginfo_stub_issue2061_getlabel, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2061, withDefault, arginfo_stub_issue2061_withdefault, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
