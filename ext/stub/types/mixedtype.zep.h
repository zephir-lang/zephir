
extern zend_class_entry *stub_types_mixedtype_ce;

ZEPHIR_INIT_CLASS(Stub_Types_MixedType);

PHP_METHOD(Stub_Types_MixedType, returnMixedObject);
PHP_METHOD(Stub_Types_MixedType, returnMixedArray);
PHP_METHOD(Stub_Types_MixedType, returnMixedString);
PHP_METHOD(Stub_Types_MixedType, returnMixedInt);
PHP_METHOD(Stub_Types_MixedType, returnMixedFloat);
PHP_METHOD(Stub_Types_MixedType, returnMixedBool);
PHP_METHOD(Stub_Types_MixedType, returnMixedNull);
PHP_METHOD(Stub_Types_MixedType, returnMixed74);
PHP_METHOD(Stub_Types_MixedType, returnMultiButAlwaysMixed);

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_types_mixedtype_returnmixedobject, 0, 0, IS_MIXED, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_types_mixedtype_returnmixedobject, 0, 0, IS_NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_types_mixedtype_returnmixedarray, 0, 0, IS_MIXED, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_types_mixedtype_returnmixedarray, 0, 0, IS_NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_types_mixedtype_returnmixedstring, 0, 0, IS_MIXED, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_types_mixedtype_returnmixedstring, 0, 0, IS_NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_types_mixedtype_returnmixedint, 0, 0, IS_MIXED, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_types_mixedtype_returnmixedint, 0, 0, IS_NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_types_mixedtype_returnmixedfloat, 0, 0, IS_MIXED, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_types_mixedtype_returnmixedfloat, 0, 0, IS_NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_types_mixedtype_returnmixedbool, 0, 0, IS_MIXED, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_types_mixedtype_returnmixedbool, 0, 0, IS_NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_types_mixedtype_returnmixednull, 0, 0, IS_MIXED, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_types_mixedtype_returnmixednull, 0, 0, IS_NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_types_mixedtype_returnmixed74, 0, 0, IS_MIXED, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_types_mixedtype_returnmixed74, 0, 0, IS_NULL, 0)
#endif
	ZEND_ARG_TYPE_INFO(0, diff, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_types_mixedtype_returnmultibutalwaysmixed, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_types_mixedtype_method_entry) {
	PHP_ME(Stub_Types_MixedType, returnMixedObject, arginfo_stub_types_mixedtype_returnmixedobject, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Types_MixedType, returnMixedArray, arginfo_stub_types_mixedtype_returnmixedarray, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Types_MixedType, returnMixedString, arginfo_stub_types_mixedtype_returnmixedstring, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Types_MixedType, returnMixedInt, arginfo_stub_types_mixedtype_returnmixedint, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Types_MixedType, returnMixedFloat, arginfo_stub_types_mixedtype_returnmixedfloat, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Types_MixedType, returnMixedBool, arginfo_stub_types_mixedtype_returnmixedbool, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Types_MixedType, returnMixedNull, arginfo_stub_types_mixedtype_returnmixednull, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Types_MixedType, returnMixed74, arginfo_stub_types_mixedtype_returnmixed74, ZEND_ACC_PUBLIC)
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Types_MixedType, returnMultiButAlwaysMixed, arginfo_stub_types_mixedtype_returnmultibutalwaysmixed, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Types_MixedType, returnMultiButAlwaysMixed, NULL, ZEND_ACC_PUBLIC)
#endif
	PHP_FE_END
};
