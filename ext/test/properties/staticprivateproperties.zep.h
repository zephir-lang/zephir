
extern zend_class_entry *test_properties_staticprivateproperties_ce;

ZEPHIR_INIT_CLASS(Test_Properties_StaticPrivateProperties);

PHP_METHOD(Test_Properties_StaticPrivateProperties, getInstance);

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_test_properties_staticprivateproperties_getinstance, 0, 0, Test\\Properties\\StaticPrivateProperties, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_properties_staticprivateproperties_getinstance, 0, 0, IS_OBJECT, "Test\\Properties\\StaticPrivateProperties", 0)
#endif
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_properties_staticprivateproperties_method_entry) {
	PHP_ME(Test_Properties_StaticPrivateProperties, getInstance, arginfo_test_properties_staticprivateproperties_getinstance, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
