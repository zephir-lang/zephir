
extern zend_class_entry *stub_properties_staticprotectedproperties_ce;

ZEPHIR_INIT_CLASS(Stub_Properties_StaticProtectedProperties);

PHP_METHOD(Stub_Properties_StaticProtectedProperties, setSomeVar);
PHP_METHOD(Stub_Properties_StaticProtectedProperties, getSomeVar);
PHP_METHOD(Stub_Properties_StaticProtectedProperties, getSomeNull);
PHP_METHOD(Stub_Properties_StaticProtectedProperties, setSomeNull);
PHP_METHOD(Stub_Properties_StaticProtectedProperties, getSomeNullInitial);
PHP_METHOD(Stub_Properties_StaticProtectedProperties, getSomeFalse);
PHP_METHOD(Stub_Properties_StaticProtectedProperties, getSomeTrue);
PHP_METHOD(Stub_Properties_StaticProtectedProperties, getSomeInteger);
PHP_METHOD(Stub_Properties_StaticProtectedProperties, getSomeDouble);
PHP_METHOD(Stub_Properties_StaticProtectedProperties, getSomeString);
PHP_METHOD(Stub_Properties_StaticProtectedProperties, compareStaticNull);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_properties_staticprotectedproperties_setsomevar, 0, 0, 1)
	ZEND_ARG_INFO(0, someVar)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_properties_staticprotectedproperties_getsomevar, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_properties_staticprotectedproperties_getsomenull, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_properties_staticprotectedproperties_setsomenull, 0, 0, 1)
	ZEND_ARG_INFO(0, param)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_properties_staticprotectedproperties_getsomenullinitial, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_properties_staticprotectedproperties_getsomefalse, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_properties_staticprotectedproperties_getsometrue, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_properties_staticprotectedproperties_getsomeinteger, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_properties_staticprotectedproperties_getsomedouble, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_properties_staticprotectedproperties_getsomestring, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_properties_staticprotectedproperties_comparestaticnull, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_properties_staticprotectedproperties_method_entry) {
	PHP_ME(Stub_Properties_StaticProtectedProperties, setSomeVar, arginfo_stub_properties_staticprotectedproperties_setsomevar, ZEND_ACC_PUBLIC)
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Properties_StaticProtectedProperties, getSomeVar, arginfo_stub_properties_staticprotectedproperties_getsomevar, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Properties_StaticProtectedProperties, getSomeVar, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Properties_StaticProtectedProperties, getSomeNull, arginfo_stub_properties_staticprotectedproperties_getsomenull, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
#else
	PHP_ME(Stub_Properties_StaticProtectedProperties, getSomeNull, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
#endif
	PHP_ME(Stub_Properties_StaticProtectedProperties, setSomeNull, arginfo_stub_properties_staticprotectedproperties_setsomenull, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Properties_StaticProtectedProperties, getSomeNullInitial, arginfo_stub_properties_staticprotectedproperties_getsomenullinitial, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
#else
	PHP_ME(Stub_Properties_StaticProtectedProperties, getSomeNullInitial, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Properties_StaticProtectedProperties, getSomeFalse, arginfo_stub_properties_staticprotectedproperties_getsomefalse, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
#else
	PHP_ME(Stub_Properties_StaticProtectedProperties, getSomeFalse, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Properties_StaticProtectedProperties, getSomeTrue, arginfo_stub_properties_staticprotectedproperties_getsometrue, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
#else
	PHP_ME(Stub_Properties_StaticProtectedProperties, getSomeTrue, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Properties_StaticProtectedProperties, getSomeInteger, arginfo_stub_properties_staticprotectedproperties_getsomeinteger, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
#else
	PHP_ME(Stub_Properties_StaticProtectedProperties, getSomeInteger, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Properties_StaticProtectedProperties, getSomeDouble, arginfo_stub_properties_staticprotectedproperties_getsomedouble, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
#else
	PHP_ME(Stub_Properties_StaticProtectedProperties, getSomeDouble, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Properties_StaticProtectedProperties, getSomeString, arginfo_stub_properties_staticprotectedproperties_getsomestring, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
#else
	PHP_ME(Stub_Properties_StaticProtectedProperties, getSomeString, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
#endif
	PHP_ME(Stub_Properties_StaticProtectedProperties, compareStaticNull, arginfo_stub_properties_staticprotectedproperties_comparestaticnull, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
