
extern zend_class_entry *stub_properties_nestedproperty_ce;

ZEPHIR_INIT_CLASS(Stub_Properties_NestedProperty);

PHP_METHOD(Stub_Properties_NestedProperty, __construct);
PHP_METHOD(Stub_Properties_NestedProperty, setData);
PHP_METHOD(Stub_Properties_NestedProperty, setName);
PHP_METHOD(Stub_Properties_NestedProperty, getBar);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_properties_nestedproperty___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_properties_nestedproperty_setdata, 0, 0, IS_VOID, 0)

	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, value, IS_LONG, 0, "0")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_properties_nestedproperty_setname, 0, 1, IS_VOID, 0)

	ZEND_ARG_TYPE_INFO(0, value, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_properties_nestedproperty_getbar, 0, 0, Stub\\Properties\\NestedPropertyTarget, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_properties_nestedproperty_method_entry) {
PHP_ME(Stub_Properties_NestedProperty, __construct, arginfo_stub_properties_nestedproperty___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Stub_Properties_NestedProperty, setData, arginfo_stub_properties_nestedproperty_setdata, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Properties_NestedProperty, setName, arginfo_stub_properties_nestedproperty_setname, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Properties_NestedProperty, getBar, arginfo_stub_properties_nestedproperty_getbar, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
