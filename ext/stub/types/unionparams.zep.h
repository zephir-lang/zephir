
extern zend_class_entry *stub_types_unionparams_ce;

ZEPHIR_INIT_CLASS(Stub_Types_UnionParams);

PHP_METHOD(Stub_Types_UnionParams, number);
PHP_METHOD(Stub_Types_UnionParams, scalar);
PHP_METHOD(Stub_Types_UnionParams, floatOrString);
PHP_METHOD(Stub_Types_UnionParams, intOrFalse);
PHP_METHOD(Stub_Types_UnionParams, objectOrArray);
PHP_METHOD(Stub_Types_UnionParams, classes);
PHP_METHOD(Stub_Types_UnionParams, second);
PHP_METHOD(Stub_Types_UnionParams, optional);
PHP_METHOD(Stub_Types_UnionParams, nullDefault);
PHP_METHOD(Stub_Types_UnionParams, staticNumber);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_types_unionparams_number, 0, 0, 1)
	ZEND_ARG_TYPE_MASK(0, value, MAY_BE_LONG|MAY_BE_DOUBLE, NULL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_types_unionparams_scalar, 0, 0, 1)
	ZEND_ARG_TYPE_MASK(0, value, MAY_BE_STRING|MAY_BE_BOOL, NULL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_types_unionparams_floatorstring, 0, 0, 1)
	ZEND_ARG_TYPE_MASK(0, value, MAY_BE_DOUBLE|MAY_BE_STRING, NULL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_types_unionparams_intorfalse, 0, 0, 1)
	ZEND_ARG_TYPE_MASK(0, value, MAY_BE_LONG|MAY_BE_FALSE, NULL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_types_unionparams_objectorarray, 0, 0, 1)
	ZEND_ARG_OBJ_TYPE_MASK(0, value, Stub\\Types\\MayBe, MAY_BE_ARRAY, NULL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_types_unionparams_classes, 0, 0, 1)
	ZEND_ARG_OBJ_TYPE_MASK(0, value, Stub\\Types\\MayBe|ArrayObject, 0, NULL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_types_unionparams_second, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, label, IS_STRING, 0)
	ZEND_ARG_TYPE_MASK(0, value, MAY_BE_LONG|MAY_BE_DOUBLE, NULL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_types_unionparams_optional, 0, 0, 0)
	ZEND_ARG_TYPE_MASK(0, value, MAY_BE_LONG|MAY_BE_STRING, "5")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_types_unionparams_nulldefault, 0, 0, 0)
	ZEND_ARG_TYPE_MASK(0, value, MAY_BE_LONG|MAY_BE_STRING|MAY_BE_NULL, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_types_unionparams_staticnumber, 0, 0, 1)
	ZEND_ARG_TYPE_MASK(0, value, MAY_BE_LONG|MAY_BE_DOUBLE, NULL)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_types_unionparams_method_entry) {
	PHP_ME(Stub_Types_UnionParams, number, arginfo_stub_types_unionparams_number, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Types_UnionParams, scalar, arginfo_stub_types_unionparams_scalar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Types_UnionParams, floatOrString, arginfo_stub_types_unionparams_floatorstring, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Types_UnionParams, intOrFalse, arginfo_stub_types_unionparams_intorfalse, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Types_UnionParams, objectOrArray, arginfo_stub_types_unionparams_objectorarray, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Types_UnionParams, classes, arginfo_stub_types_unionparams_classes, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Types_UnionParams, second, arginfo_stub_types_unionparams_second, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Types_UnionParams, optional, arginfo_stub_types_unionparams_optional, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Types_UnionParams, nullDefault, arginfo_stub_types_unionparams_nulldefault, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Types_UnionParams, staticNumber, arginfo_stub_types_unionparams_staticnumber, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
