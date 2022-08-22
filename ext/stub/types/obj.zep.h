
extern zend_class_entry *stub_types_obj_ce;

ZEPHIR_INIT_CLASS(Stub_Types_Obj);

PHP_METHOD(Stub_Types_Obj, nullableObjectReturnNull);
PHP_METHOD(Stub_Types_Obj, nullableObjectReturnObj);
PHP_METHOD(Stub_Types_Obj, objectReturn);

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_stub_types_obj_nullableobjectreturnnull, 0, 0, MAY_BE_NULL|MAY_BE_OBJECT)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_types_obj_nullableobjectreturnnull, 0, 0, IS_OBJECT, 1)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_stub_types_obj_nullableobjectreturnobj, 0, 0, MAY_BE_NULL|MAY_BE_OBJECT)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_types_obj_nullableobjectreturnobj, 0, 0, IS_OBJECT, 1)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 80000
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_stub_types_obj_objectreturn, 0, 0, MAY_BE_OBJECT)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_types_obj_objectreturn, 0, 0, IS_OBJECT, 0)
#endif
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_types_obj_method_entry) {
	PHP_ME(Stub_Types_Obj, nullableObjectReturnNull, arginfo_stub_types_obj_nullableobjectreturnnull, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Types_Obj, nullableObjectReturnObj, arginfo_stub_types_obj_nullableobjectreturnobj, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Types_Obj, objectReturn, arginfo_stub_types_obj_objectreturn, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
