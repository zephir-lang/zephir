
extern zend_class_entry *test_properties_staticpublicproperties_ce;

ZEPHIR_INIT_CLASS(Test_Properties_StaticPublicProperties);

PHP_METHOD(Test_Properties_StaticPublicProperties, setSomeString);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_properties_staticpublicproperties_setsomestring, 0, 0, 1)
	ZEND_ARG_INFO(0, val)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_properties_staticpublicproperties_method_entry) {
	PHP_ME(Test_Properties_StaticPublicProperties, setSomeString, arginfo_test_properties_staticpublicproperties_setsomestring, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
