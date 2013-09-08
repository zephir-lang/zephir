
extern zend_class_entry *test_properties_protectedproperties_ce;

ZEPHIR_INIT_CLASS(Test_Properties_ProtectedProperties);

PHP_METHOD(Test_Properties_ProtectedProperties, getSomeNull);
PHP_METHOD(Test_Properties_ProtectedProperties, getSomeNullInitial);
PHP_METHOD(Test_Properties_ProtectedProperties, getSomeFalse);
PHP_METHOD(Test_Properties_ProtectedProperties, getSomeTrue);
PHP_METHOD(Test_Properties_ProtectedProperties, getSomeInteger);
PHP_METHOD(Test_Properties_ProtectedProperties, getSomeDouble);
PHP_METHOD(Test_Properties_ProtectedProperties, getSomeString);

ZEPHIR_INIT_FUNCS(test_properties_protectedproperties_method_entry) {
	PHP_ME(Test_Properties_ProtectedProperties, getSomeNull, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Properties_ProtectedProperties, getSomeNullInitial, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Properties_ProtectedProperties, getSomeFalse, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Properties_ProtectedProperties, getSomeTrue, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Properties_ProtectedProperties, getSomeInteger, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Properties_ProtectedProperties, getSomeDouble, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Properties_ProtectedProperties, getSomeString, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
