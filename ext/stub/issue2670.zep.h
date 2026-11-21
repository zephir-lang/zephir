
extern zend_class_entry *stub_issue2670_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2670);

PHP_METHOD(Stub_Issue2670, uniqueKeyLoop);
PHP_METHOD(Stub_Issue2670, emptyValue);
PHP_METHOD(Stub_Issue2670, issetValue);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2670_uniquekeyloop, 0, 0, 3)
	ZEND_ARG_INFO(0, prefix)
	ZEND_ARG_INFO(0, value)
	ZEND_ARG_TYPE_INFO(0, count, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2670_emptyvalue, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, obj)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2670_issetvalue, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, obj)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2670_method_entry) {
	PHP_ME(Stub_Issue2670, uniqueKeyLoop, arginfo_stub_issue2670_uniquekeyloop, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2670, emptyValue, arginfo_stub_issue2670_emptyvalue, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2670, issetValue, arginfo_stub_issue2670_issetvalue, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
