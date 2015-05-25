
extern zend_class_entry *test_properties_propertyarray_ce;

ZEPHIR_INIT_CLASS(Test_Properties_PropertyArray);

PHP_METHOD(Test_Properties_PropertyArray, __construct);

ZEPHIR_INIT_FUNCS(test_properties_propertyarray_method_entry) {
	PHP_ME(Test_Properties_PropertyArray, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_FE_END
};
