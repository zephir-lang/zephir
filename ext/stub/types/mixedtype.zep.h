
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
PHP_METHOD(Stub_Types_MixedType, paramMixed);
PHP_METHOD(Stub_Types_MixedType, paramMixedTwo);
PHP_METHOD(Stub_Types_MixedType, paramMixedWithMulti);
PHP_METHOD(Stub_Types_MixedType, paramAndReturnMixed);
PHP_METHOD(Stub_Types_MixedType, castToStringMixedAndReturnMixed);
PHP_METHOD(Stub_Types_MixedType, castToStringInternallyMixedAndReturnMixed);
PHP_METHOD(Stub_Types_MixedType, castToIntMixedAndReturnMixed);
PHP_METHOD(Stub_Types_MixedType, castToIntInternallyMixedAndReturnMixed);
PHP_METHOD(Stub_Types_MixedType, castToBoolMixedAndReturnMixed);
PHP_METHOD(Stub_Types_MixedType, castToBoolInternallyMixedAndReturnMixed);
PHP_METHOD(Stub_Types_MixedType, castToFloatMixedAndReturnMixed);
PHP_METHOD(Stub_Types_MixedType, castToFloatInternallyMixedAndReturnMixed);
PHP_METHOD(Stub_Types_MixedType, mixedInCondition);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_types_mixedtype_returnmixedobject, 0, 0, IS_MIXED, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_types_mixedtype_returnmixedarray, 0, 0, IS_MIXED, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_types_mixedtype_returnmixedstring, 0, 0, IS_MIXED, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_types_mixedtype_returnmixedint, 0, 0, IS_MIXED, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_types_mixedtype_returnmixedfloat, 0, 0, IS_MIXED, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_types_mixedtype_returnmixedbool, 0, 0, IS_MIXED, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_types_mixedtype_returnmixednull, 0, 0, IS_MIXED, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_types_mixedtype_returnmixed74, 0, 0, IS_MIXED, 0)
	ZEND_ARG_TYPE_INFO(0, diff, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_types_mixedtype_returnmultibutalwaysmixed, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_types_mixedtype_parammixed, 0, 0, 1)
	ZEND_ARG_INFO(0, val)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_types_mixedtype_parammixedtwo, 0, 0, 2)
	ZEND_ARG_INFO(0, val1)
	ZEND_ARG_INFO(0, val2)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_types_mixedtype_parammixedwithmulti, 0, 0, 3)
	ZEND_ARG_TYPE_INFO(0, intVal, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, stringVal, IS_STRING, 0)
	ZEND_ARG_INFO(0, mixedVal)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_types_mixedtype_paramandreturnmixed, 0, 1, IS_MIXED, 0)
	ZEND_ARG_INFO(0, val)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_types_mixedtype_casttostringmixedandreturnmixed, 0, 1, IS_MIXED, 0)
	ZEND_ARG_INFO(0, val)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_types_mixedtype_casttostringinternallymixedandreturnmixed, 0, 1, IS_MIXED, 0)
	ZEND_ARG_INFO(0, val)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_types_mixedtype_casttointmixedandreturnmixed, 0, 1, IS_MIXED, 0)
	ZEND_ARG_INFO(0, val)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_types_mixedtype_casttointinternallymixedandreturnmixed, 0, 1, IS_MIXED, 0)
	ZEND_ARG_INFO(0, val)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_types_mixedtype_casttoboolmixedandreturnmixed, 0, 1, IS_MIXED, 0)
	ZEND_ARG_INFO(0, val)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_types_mixedtype_casttoboolinternallymixedandreturnmixed, 0, 1, IS_MIXED, 0)
	ZEND_ARG_INFO(0, val)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_types_mixedtype_casttofloatmixedandreturnmixed, 0, 1, IS_MIXED, 0)
	ZEND_ARG_INFO(0, val)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_types_mixedtype_casttofloatinternallymixedandreturnmixed, 0, 1, IS_MIXED, 0)
	ZEND_ARG_INFO(0, val)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_types_mixedtype_mixedincondition, 0, 1, IS_VOID, 0)

	ZEND_ARG_INFO(0, val)
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
PHP_ME(Stub_Types_MixedType, returnMultiButAlwaysMixed, arginfo_stub_types_mixedtype_returnmultibutalwaysmixed, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Types_MixedType, paramMixed, arginfo_stub_types_mixedtype_parammixed, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Types_MixedType, paramMixedTwo, arginfo_stub_types_mixedtype_parammixedtwo, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Types_MixedType, paramMixedWithMulti, arginfo_stub_types_mixedtype_parammixedwithmulti, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Types_MixedType, paramAndReturnMixed, arginfo_stub_types_mixedtype_paramandreturnmixed, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Types_MixedType, castToStringMixedAndReturnMixed, arginfo_stub_types_mixedtype_casttostringmixedandreturnmixed, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Types_MixedType, castToStringInternallyMixedAndReturnMixed, arginfo_stub_types_mixedtype_casttostringinternallymixedandreturnmixed, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Types_MixedType, castToIntMixedAndReturnMixed, arginfo_stub_types_mixedtype_casttointmixedandreturnmixed, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Types_MixedType, castToIntInternallyMixedAndReturnMixed, arginfo_stub_types_mixedtype_casttointinternallymixedandreturnmixed, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Types_MixedType, castToBoolMixedAndReturnMixed, arginfo_stub_types_mixedtype_casttoboolmixedandreturnmixed, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Types_MixedType, castToBoolInternallyMixedAndReturnMixed, arginfo_stub_types_mixedtype_casttoboolinternallymixedandreturnmixed, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Types_MixedType, castToFloatMixedAndReturnMixed, arginfo_stub_types_mixedtype_casttofloatmixedandreturnmixed, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Types_MixedType, castToFloatInternallyMixedAndReturnMixed, arginfo_stub_types_mixedtype_casttofloatinternallymixedandreturnmixed, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Types_MixedType, mixedInCondition, arginfo_stub_types_mixedtype_mixedincondition, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
