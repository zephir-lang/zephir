
extern zend_class_entry *stub_properties_staticpublicproperties_ce;

ZEPHIR_INIT_CLASS(Stub_Properties_StaticPublicProperties);

PHP_METHOD(Stub_Properties_StaticPublicProperties, setSomeString);
PHP_METHOD(Stub_Properties_StaticPublicProperties, testAddAndSub);
PHP_METHOD(Stub_Properties_StaticPublicProperties, testAddAndSub2);
PHP_METHOD(Stub_Properties_StaticPublicProperties, testAddAndSub3);
PHP_METHOD(Stub_Properties_StaticPublicProperties, testAddAndSub4);
PHP_METHOD(Stub_Properties_StaticPublicProperties, testAddAndSub5);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_properties_staticpublicproperties_setsomestring, 0, 0, 1)
	ZEND_ARG_INFO(0, val)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_properties_staticpublicproperties_testaddandsub, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_properties_staticpublicproperties_testaddandsub2, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_properties_staticpublicproperties_testaddandsub3, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_properties_staticpublicproperties_testaddandsub4, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_properties_staticpublicproperties_testaddandsub5, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_properties_staticpublicproperties_method_entry) {
	PHP_ME(Stub_Properties_StaticPublicProperties, setSomeString, arginfo_stub_properties_staticpublicproperties_setsomestring, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Properties_StaticPublicProperties, testAddAndSub, arginfo_stub_properties_staticpublicproperties_testaddandsub, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
#else
	PHP_ME(Stub_Properties_StaticPublicProperties, testAddAndSub, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Properties_StaticPublicProperties, testAddAndSub2, arginfo_stub_properties_staticpublicproperties_testaddandsub2, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
#else
	PHP_ME(Stub_Properties_StaticPublicProperties, testAddAndSub2, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Properties_StaticPublicProperties, testAddAndSub3, arginfo_stub_properties_staticpublicproperties_testaddandsub3, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
#else
	PHP_ME(Stub_Properties_StaticPublicProperties, testAddAndSub3, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Properties_StaticPublicProperties, testAddAndSub4, arginfo_stub_properties_staticpublicproperties_testaddandsub4, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
#else
	PHP_ME(Stub_Properties_StaticPublicProperties, testAddAndSub4, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Properties_StaticPublicProperties, testAddAndSub5, arginfo_stub_properties_staticpublicproperties_testaddandsub5, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
#else
	PHP_ME(Stub_Properties_StaticPublicProperties, testAddAndSub5, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
#endif
	PHP_FE_END
};
