
extern zend_class_entry *typehints_retval_ce;

ZEPHIR_INIT_CLASS(TypeHints_RetVal);

PHP_METHOD(TypeHints_RetVal, getMyVar);
PHP_METHOD(TypeHints_RetVal, getMyString);
PHP_METHOD(TypeHints_RetVal, getMyArray);
PHP_METHOD(TypeHints_RetVal, retval_var_var);
PHP_METHOD(TypeHints_RetVal, retval_var_var_builit_1);
PHP_METHOD(TypeHints_RetVal, retval_var_var_builit_2);
PHP_METHOD(TypeHints_RetVal, retval_var);
PHP_METHOD(TypeHints_RetVal, retval_string);
PHP_METHOD(TypeHints_RetVal, retval_boolean);
PHP_METHOD(TypeHints_RetVal, retval_int);
PHP_METHOD(TypeHints_RetVal, retval_long);
PHP_METHOD(TypeHints_RetVal, retval_double);
PHP_METHOD(TypeHints_RetVal, retval_array);
PHP_METHOD(TypeHints_RetVal, retval_array_of_objects);
PHP_METHOD(TypeHints_RetVal, retval_collection);
PHP_METHOD(TypeHints_RetVal, retval_retval);
PHP_METHOD(TypeHints_RetVal, retval_nullable_int);
PHP_METHOD(TypeHints_RetVal, retval_nullable_uint);
PHP_METHOD(TypeHints_RetVal, retval_nullable_long);
PHP_METHOD(TypeHints_RetVal, retval_nullable_float);
PHP_METHOD(TypeHints_RetVal, retval_nullable_double);
PHP_METHOD(TypeHints_RetVal, retval_nullable_string);
PHP_METHOD(TypeHints_RetVal, retval_nullable_boolean);
PHP_METHOD(TypeHints_RetVal, retval_nullable_char);
PHP_METHOD(TypeHints_RetVal, retval_nullable_array);
PHP_METHOD(TypeHints_RetVal, retval_object_or_scalar);
PHP_METHOD(TypeHints_RetVal, retval_static_object_or_scalar);

ZEND_BEGIN_ARG_INFO_EX(arginfo_typehints_retval_getmyvar, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_typehints_retval_getmystring, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_typehints_retval_getmyarray, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_typehints_retval_retval_var_var, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_typehints_retval_retval_var_var_builit_1, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_typehints_retval_retval_var_var_builit_2, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_typehints_retval_retval_var, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_typehints_retval_retval_string, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_typehints_retval_retval_boolean, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_typehints_retval_retval_int, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_typehints_retval_retval_long, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_typehints_retval_retval_double, 0, 0, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_typehints_retval_retval_array, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_typehints_retval_retval_array_of_objects, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_typehints_retval_retval_collection, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_typehints_retval_retval_retval, 0, 0, TypeHints\\RetVal, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_typehints_retval_retval_nullable_int, 0, 0, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_typehints_retval_retval_nullable_uint, 0, 0, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_typehints_retval_retval_nullable_long, 0, 0, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_typehints_retval_retval_nullable_float, 0, 0, IS_DOUBLE, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_typehints_retval_retval_nullable_double, 0, 0, IS_DOUBLE, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_typehints_retval_retval_nullable_string, 0, 0, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_typehints_retval_retval_nullable_boolean, 0, 0, _IS_BOOL, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_typehints_retval_retval_nullable_char, 0, 0, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_typehints_retval_retval_nullable_array, 0, 0, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_typehints_retval_retval_object_or_scalar, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_typehints_retval_retval_static_object_or_scalar, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(typehints_retval_method_entry) {
	PHP_ME(TypeHints_RetVal, getMyVar, arginfo_typehints_retval_getmyvar, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, getMyString, arginfo_typehints_retval_getmystring, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, getMyArray, arginfo_typehints_retval_getmyarray, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_var_var, arginfo_typehints_retval_retval_var_var, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_var_var_builit_1, arginfo_typehints_retval_retval_var_var_builit_1, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_var_var_builit_2, arginfo_typehints_retval_retval_var_var_builit_2, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_var, arginfo_typehints_retval_retval_var, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_string, arginfo_typehints_retval_retval_string, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_boolean, arginfo_typehints_retval_retval_boolean, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_int, arginfo_typehints_retval_retval_int, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_long, arginfo_typehints_retval_retval_long, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_double, arginfo_typehints_retval_retval_double, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_array, arginfo_typehints_retval_retval_array, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_array_of_objects, arginfo_typehints_retval_retval_array_of_objects, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_collection, arginfo_typehints_retval_retval_collection, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_retval, arginfo_typehints_retval_retval_retval, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_nullable_int, arginfo_typehints_retval_retval_nullable_int, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_nullable_uint, arginfo_typehints_retval_retval_nullable_uint, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_nullable_long, arginfo_typehints_retval_retval_nullable_long, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_nullable_float, arginfo_typehints_retval_retval_nullable_float, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_nullable_double, arginfo_typehints_retval_retval_nullable_double, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_nullable_string, arginfo_typehints_retval_retval_nullable_string, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_nullable_boolean, arginfo_typehints_retval_retval_nullable_boolean, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_nullable_char, arginfo_typehints_retval_retval_nullable_char, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_nullable_array, arginfo_typehints_retval_retval_nullable_array, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_object_or_scalar, arginfo_typehints_retval_retval_object_or_scalar, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_static_object_or_scalar, arginfo_typehints_retval_retval_static_object_or_scalar, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
