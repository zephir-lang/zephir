
extern zend_class_entry *stub_issue2546_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2546);

PHP_METHOD(Stub_Issue2546, collectExcluding);
PHP_METHOD(Stub_Issue2546, collectUntil);
PHP_METHOD(Stub_Issue2546, collectKeysExcluding);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2546_collectexcluding, 0, 2, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, items)
	ZEND_ARG_INFO(0, skip)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2546_collectuntil, 0, 2, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, items)
	ZEND_ARG_INFO(0, stop)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2546_collectkeysexcluding, 0, 2, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, items)
	ZEND_ARG_INFO(0, skipKey)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2546_method_entry) {
	PHP_ME(Stub_Issue2546, collectExcluding, arginfo_stub_issue2546_collectexcluding, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2546, collectUntil, arginfo_stub_issue2546_collectuntil, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2546, collectKeysExcluding, arginfo_stub_issue2546_collectkeysexcluding, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
