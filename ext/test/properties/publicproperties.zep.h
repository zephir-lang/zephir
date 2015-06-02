
extern zend_class_entry *test_properties_publicproperties_ce;

ZEPHIR_INIT_CLASS(Test_Properties_PublicProperties);

PHP_METHOD(Test_Properties_PublicProperties, setSomeGetterSetterArray);
PHP_METHOD(Test_Properties_PublicProperties, getSomeGetterSetterArray);
PHP_METHOD(Test_Properties_PublicProperties, test394Issue);
PHP_METHOD(Test_Properties_PublicProperties, __construct);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_properties_publicproperties_setsomegettersetterarray, 0, 0, 1)
	ZEND_ARG_INFO(0, someGetterSetterArray)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_properties_publicproperties_method_entry) {
	PHP_ME(Test_Properties_PublicProperties, setSomeGetterSetterArray, arginfo_test_properties_publicproperties_setsomegettersetterarray, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Properties_PublicProperties, getSomeGetterSetterArray, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Properties_PublicProperties, test394Issue, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Properties_PublicProperties, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_FE_END
};
