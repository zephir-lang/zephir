
extern zend_class_entry *stub_traits_nameable_ce;

ZEPHIR_INIT_CLASS(Stub_Traits_Nameable);

PHP_METHOD(Stub_Traits_Nameable, setName);
PHP_METHOD(Stub_Traits_Nameable, getName);
PHP_METHOD(Stub_Traits_Nameable, getTags);
PHP_METHOD(Stub_Traits_Nameable, addTag);
PHP_METHOD(Stub_Traits_Nameable, getMeta);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_traits_nameable_setname, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_traits_nameable_getname, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_traits_nameable_gettags, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_traits_nameable_addtag, 0, 0, 1)
	ZEND_ARG_INFO(0, tag)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_traits_nameable_getmeta, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_traits_nameable_method_entry) {
	PHP_ME(Stub_Traits_Nameable, setName, arginfo_stub_traits_nameable_setname, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Traits_Nameable, getName, arginfo_stub_traits_nameable_getname, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Traits_Nameable, getTags, arginfo_stub_traits_nameable_gettags, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Traits_Nameable, addTag, arginfo_stub_traits_nameable_addtag, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Traits_Nameable, getMeta, arginfo_stub_traits_nameable_getmeta, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
