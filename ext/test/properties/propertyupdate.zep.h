
extern zend_class_entry *test_properties_propertyupdate_ce;

ZEPHIR_INIT_CLASS(Test_Properties_PropertyUpdate);

PHP_METHOD(Test_Properties_PropertyUpdate, update1);

ZEPHIR_INIT_FUNCS(test_properties_propertyupdate_method_entry) {
	PHP_ME(Test_Properties_PropertyUpdate, update1, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
