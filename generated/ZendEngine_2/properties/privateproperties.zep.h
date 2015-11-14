
extern zend_class_entry *test_properties_privateproperties_ce;

ZEPHIR_INIT_CLASS(Test_Properties_PrivateProperties);

PHP_METHOD(Test_Properties_PrivateProperties, getSomeNull);
PHP_METHOD(Test_Properties_PrivateProperties, getSomeNullInitial);
PHP_METHOD(Test_Properties_PrivateProperties, getSomeFalse);
PHP_METHOD(Test_Properties_PrivateProperties, getSomeTrue);
PHP_METHOD(Test_Properties_PrivateProperties, getSomeInteger);
PHP_METHOD(Test_Properties_PrivateProperties, getSomeDouble);
PHP_METHOD(Test_Properties_PrivateProperties, getSomeString);

ZEPHIR_INIT_FUNCS(test_properties_privateproperties_method_entry) {
	PHP_ME(Test_Properties_PrivateProperties, getSomeNull, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Properties_PrivateProperties, getSomeNullInitial, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Properties_PrivateProperties, getSomeFalse, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Properties_PrivateProperties, getSomeTrue, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Properties_PrivateProperties, getSomeInteger, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Properties_PrivateProperties, getSomeDouble, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Properties_PrivateProperties, getSomeString, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
