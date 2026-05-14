
extern zend_class_entry *stub_arrayaccessarr_ce;

ZEPHIR_INIT_CLASS(Stub_ArrayAccessArr);

PHP_METHOD(Stub_ArrayAccessArr, __construct);
PHP_METHOD(Stub_ArrayAccessArr, offsetSet);
PHP_METHOD(Stub_ArrayAccessArr, offsetExists);
PHP_METHOD(Stub_ArrayAccessArr, offsetUnset);
PHP_METHOD(Stub_ArrayAccessArr, offsetGet);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_arrayaccessarr___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_arrayaccessarr_offsetset, 0, 2, IS_VOID, 0)

	ZEND_ARG_INFO(0, offset)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_arrayaccessarr_offsetexists, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, offset)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_arrayaccessarr_offsetunset, 0, 1, IS_VOID, 0)

	ZEND_ARG_INFO(0, offset)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_arrayaccessarr_offsetget, 0, 1, IS_MIXED, 0)
	ZEND_ARG_INFO(0, offset)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_arrayaccessarr_method_entry) {
PHP_ME(Stub_ArrayAccessArr, __construct, arginfo_stub_arrayaccessarr___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Stub_ArrayAccessArr, offsetSet, arginfo_stub_arrayaccessarr_offsetset, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayAccessArr, offsetExists, arginfo_stub_arrayaccessarr_offsetexists, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayAccessArr, offsetUnset, arginfo_stub_arrayaccessarr_offsetunset, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayAccessArr, offsetGet, arginfo_stub_arrayaccessarr_offsetget, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
