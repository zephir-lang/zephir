
extern zend_class_entry *stub_properties_typedproperties_ce;

ZEPHIR_INIT_CLASS(Stub_Properties_TypedProperties);

PHP_METHOD(Stub_Properties_TypedProperties, getVal);
PHP_METHOD(Stub_Properties_TypedProperties, getMaybeNum);
PHP_METHOD(Stub_Properties_TypedProperties, getDefaultFormatter);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_properties_typedproperties_getval, 0, 0, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_properties_typedproperties_getmaybenum, 0, 0, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_properties_typedproperties_getdefaultformatter, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_properties_typedproperties_method_entry) {
	PHP_ME(Stub_Properties_TypedProperties, getVal, arginfo_stub_properties_typedproperties_getval, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Properties_TypedProperties, getMaybeNum, arginfo_stub_properties_typedproperties_getmaybenum, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Properties_TypedProperties, getDefaultFormatter, arginfo_stub_properties_typedproperties_getdefaultformatter, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
