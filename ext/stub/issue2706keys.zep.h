
extern zend_class_entry *stub_issue2706keys_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2706Keys);

PHP_METHOD(Stub_Issue2706Keys, getTypedKeys);
PHP_METHOD(Stub_Issue2706Keys, getStaticKeys);
zend_object *zephir_init_properties_Stub_Issue2706Keys(zend_class_entry *class_type);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2706keys_gettypedkeys, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2706keys_getstatickeys, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2706keys_zephir_init_properties_stub_issue2706keys, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2706keys_zephir_init_static_properties_stub_issue2706keys, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2706keys_method_entry) {
	PHP_ME(Stub_Issue2706Keys, getTypedKeys, arginfo_stub_issue2706keys_gettypedkeys, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2706Keys, getStaticKeys, arginfo_stub_issue2706keys_getstatickeys, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
