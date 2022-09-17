
extern zend_class_entry *stub_arrayaccessobj_ce;

ZEPHIR_INIT_CLASS(Stub_ArrayAccessObj);

PHP_METHOD(Stub_ArrayAccessObj, __construct);
PHP_METHOD(Stub_ArrayAccessObj, offsetSet);
PHP_METHOD(Stub_ArrayAccessObj, offsetExists);
PHP_METHOD(Stub_ArrayAccessObj, offsetUnset);
PHP_METHOD(Stub_ArrayAccessObj, offsetGet);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_arrayaccessobj___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_arrayaccessobj_offsetset, 0, 2, IS_VOID, 0)

	ZEND_ARG_INFO(0, offset)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_arrayaccessobj_offsetexists, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, offset)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_arrayaccessobj_offsetunset, 0, 1, IS_VOID, 0)

	ZEND_ARG_INFO(0, offset)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_arrayaccessobj_offsetget, 0, 1, IS_MIXED, 0)
#else
ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_arrayaccessobj_offsetget, 0, 0, 1)
#endif
	ZEND_ARG_INFO(0, offset)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_arrayaccessobj_method_entry) {
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_ArrayAccessObj, __construct, arginfo_stub_arrayaccessobj___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
#else
	PHP_ME(Stub_ArrayAccessObj, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
#endif
	PHP_ME(Stub_ArrayAccessObj, offsetSet, arginfo_stub_arrayaccessobj_offsetset, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayAccessObj, offsetExists, arginfo_stub_arrayaccessobj_offsetexists, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayAccessObj, offsetUnset, arginfo_stub_arrayaccessobj_offsetunset, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayAccessObj, offsetGet, arginfo_stub_arrayaccessobj_offsetget, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
