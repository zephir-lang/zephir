
extern zend_class_entry *stub_oo_oonativeimplements_ce;

ZEPHIR_INIT_CLASS(Stub_Oo_OoNativeImplements);

PHP_METHOD(Stub_Oo_OoNativeImplements, count);
PHP_METHOD(Stub_Oo_OoNativeImplements, current);
PHP_METHOD(Stub_Oo_OoNativeImplements, key);
PHP_METHOD(Stub_Oo_OoNativeImplements, next);
PHP_METHOD(Stub_Oo_OoNativeImplements, rewind);
PHP_METHOD(Stub_Oo_OoNativeImplements, valid);
PHP_METHOD(Stub_Oo_OoNativeImplements, getInnerIterator);
PHP_METHOD(Stub_Oo_OoNativeImplements, getChildren);
PHP_METHOD(Stub_Oo_OoNativeImplements, hasChildren);
PHP_METHOD(Stub_Oo_OoNativeImplements, seek);
PHP_METHOD(Stub_Oo_OoNativeImplements, getIterator);
PHP_METHOD(Stub_Oo_OoNativeImplements, offsetExists);
PHP_METHOD(Stub_Oo_OoNativeImplements, offsetGet);
PHP_METHOD(Stub_Oo_OoNativeImplements, offsetSet);
PHP_METHOD(Stub_Oo_OoNativeImplements, offsetUnset);
PHP_METHOD(Stub_Oo_OoNativeImplements, serialize);
PHP_METHOD(Stub_Oo_OoNativeImplements, unserialize);
PHP_METHOD(Stub_Oo_OoNativeImplements, __serialize);
PHP_METHOD(Stub_Oo_OoNativeImplements, __unserialize);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_oo_oonativeimplements_count, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_oo_oonativeimplements_current, 0, 0, IS_MIXED, 0)
#else
ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_oonativeimplements_current, 0, 0, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_oo_oonativeimplements_key, 0, 0, IS_MIXED, 0)
#else
ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_oonativeimplements_key, 0, 0, 0)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_oo_oonativeimplements_next, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_oo_oonativeimplements_rewind, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_oo_oonativeimplements_valid, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_oo_oonativeimplements_getinneriterator, 0, 0, Iterator, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_oo_oonativeimplements_getchildren, 0, 0, RecursiveIterator, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_oo_oonativeimplements_haschildren, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_oo_oonativeimplements_seek, 0, 1, IS_VOID, 0)

	ZEND_ARG_TYPE_INFO(0, position, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_oo_oonativeimplements_getiterator, 0, 0, Traversable, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_oo_oonativeimplements_offsetexists, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, offset)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_oonativeimplements_offsetget, 0, 0, 1)
	ZEND_ARG_INFO(0, offset)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_oonativeimplements_offsetset, 0, 0, 2)
	ZEND_ARG_INFO(0, offset)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_oonativeimplements_offsetunset, 0, 0, 1)
	ZEND_ARG_INFO(0, offset)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_oo_oonativeimplements_serialize, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_oo_oonativeimplements_unserialize, 0, 1, IS_VOID, 0)
    ZEND_ARG_TYPE_INFO(0, serialized, IS_STRING, 0)
ZEND_END_ARG_INFO()
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_oo_oonativeimplements_unserialize, 0, 1, IS_VOID, 0)
    ZEND_ARG_INFO(0, serialized)
ZEND_END_ARG_INFO()
#endif

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_oo_oonativeimplements___serialize, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_oo_oonativeimplements___unserialize, 0, 1, IS_VOID, 0)

	ZEND_ARG_ARRAY_INFO(0, data, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_oo_oonativeimplements_method_entry) {
	PHP_ME(Stub_Oo_OoNativeImplements, count, arginfo_stub_oo_oonativeimplements_count, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_OoNativeImplements, current, arginfo_stub_oo_oonativeimplements_current, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_OoNativeImplements, key, arginfo_stub_oo_oonativeimplements_key, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_OoNativeImplements, next, arginfo_stub_oo_oonativeimplements_next, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_OoNativeImplements, rewind, arginfo_stub_oo_oonativeimplements_rewind, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_OoNativeImplements, valid, arginfo_stub_oo_oonativeimplements_valid, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_OoNativeImplements, getInnerIterator, arginfo_stub_oo_oonativeimplements_getinneriterator, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_OoNativeImplements, getChildren, arginfo_stub_oo_oonativeimplements_getchildren, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_OoNativeImplements, hasChildren, arginfo_stub_oo_oonativeimplements_haschildren, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_OoNativeImplements, seek, arginfo_stub_oo_oonativeimplements_seek, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_OoNativeImplements, getIterator, arginfo_stub_oo_oonativeimplements_getiterator, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_OoNativeImplements, offsetExists, arginfo_stub_oo_oonativeimplements_offsetexists, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_OoNativeImplements, offsetGet, arginfo_stub_oo_oonativeimplements_offsetget, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_OoNativeImplements, offsetSet, arginfo_stub_oo_oonativeimplements_offsetset, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_OoNativeImplements, offsetUnset, arginfo_stub_oo_oonativeimplements_offsetunset, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_OoNativeImplements, serialize, arginfo_stub_oo_oonativeimplements_serialize, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_OoNativeImplements, unserialize, arginfo_stub_oo_oonativeimplements_unserialize, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_OoNativeImplements, __serialize, arginfo_stub_oo_oonativeimplements___serialize, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_OoNativeImplements, __unserialize, arginfo_stub_oo_oonativeimplements___unserialize, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
