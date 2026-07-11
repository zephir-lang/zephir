
extern zend_class_entry *stub_propertycachemagic_ce;

ZEPHIR_INIT_CLASS(Stub_PropertyCacheMagic);

PHP_METHOD(Stub_PropertyCacheMagic, __get);
PHP_METHOD(Stub_PropertyCacheMagic, __set);
PHP_METHOD(Stub_PropertyCacheMagic, __isset);
PHP_METHOD(Stub_PropertyCacheMagic, __unset);
PHP_METHOD(Stub_PropertyCacheMagic, readVirtual);
PHP_METHOD(Stub_PropertyCacheMagic, writeVirtual);
zend_object *zephir_init_properties_Stub_PropertyCacheMagic(zend_class_entry *class_type);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_propertycachemagic___get, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_propertycachemagic___set, 0, 2, IS_VOID, 0)

	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_propertycachemagic___isset, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_propertycachemagic___unset, 0, 1, IS_VOID, 0)

	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_propertycachemagic_readvirtual, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_propertycachemagic_writevirtual, 0, 2, IS_VOID, 0)

	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_propertycachemagic_zephir_init_properties_stub_propertycachemagic, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_propertycachemagic_method_entry) {
	PHP_ME(Stub_PropertyCacheMagic, __get, arginfo_stub_propertycachemagic___get, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_PropertyCacheMagic, __set, arginfo_stub_propertycachemagic___set, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_PropertyCacheMagic, __isset, arginfo_stub_propertycachemagic___isset, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_PropertyCacheMagic, __unset, arginfo_stub_propertycachemagic___unset, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_PropertyCacheMagic, readVirtual, arginfo_stub_propertycachemagic_readvirtual, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_PropertyCacheMagic, writeVirtual, arginfo_stub_propertycachemagic_writevirtual, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
