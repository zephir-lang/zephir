
extern zend_class_entry *test_properties_extendspublicproperties_ce;

ZEPHIR_INIT_CLASS(Test_Properties_ExtendsPublicProperties);

PHP_METHOD(Test_Properties_ExtendsPublicProperties, __construct);

ZEPHIR_INIT_FUNCS(test_properties_extendspublicproperties_method_entry) {
	PHP_ME(Test_Properties_ExtendsPublicProperties, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_FE_END
};
