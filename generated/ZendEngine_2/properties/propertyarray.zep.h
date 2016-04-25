
extern zend_class_entry *test_properties_propertyarray_ce;

ZEPHIR_INIT_CLASS(Test_Properties_PropertyArray);

PHP_METHOD(Test_Properties_PropertyArray, __construct);
zend_object_value zephir_init_properties_Test_Properties_PropertyArray(zend_class_entry *class_type TSRMLS_DC);

ZEPHIR_INIT_FUNCS(test_properties_propertyarray_method_entry) {
	PHP_ME(Test_Properties_PropertyArray, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_FE_END
};
