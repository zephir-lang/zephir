
extern zend_class_entry *stub_properties_propertyarray_ce;

ZEPHIR_INIT_CLASS(Stub_Properties_PropertyArray);

PHP_METHOD(Stub_Properties_PropertyArray, __construct);
PHP_METHOD(Stub_Properties_PropertyArray, appendSome);
PHP_METHOD(Stub_Properties_PropertyArray, setOtherArray);
PHP_METHOD(Stub_Properties_PropertyArray, getOtherArray);
PHP_METHOD(Stub_Properties_PropertyArray, testIssues1831);
zend_object *zephir_init_properties_Stub_Properties_PropertyArray(zend_class_entry *class_type);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_properties_propertyarray_appendsome, 0, 0, 1)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_properties_propertyarray_setotherarray, 0, 0, 1)
	ZEND_ARG_INFO(0, arr)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_properties_propertyarray_method_entry) {
	PHP_ME(Stub_Properties_PropertyArray, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Stub_Properties_PropertyArray, appendSome, arginfo_stub_properties_propertyarray_appendsome, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Properties_PropertyArray, setOtherArray, arginfo_stub_properties_propertyarray_setotherarray, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Properties_PropertyArray, getOtherArray, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Properties_PropertyArray, testIssues1831, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
