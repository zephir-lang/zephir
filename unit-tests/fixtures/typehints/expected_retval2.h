
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

ZEPHIR_INIT_FUNCS(typehints_retval_method_entry) {
	PHP_ME(TypeHints_RetVal, getMyVar, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, getMyString, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, getMyArray, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_var_var, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_var_var_builit_1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_var_var_builit_2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_var, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_string, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_boolean, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_int, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_long, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_double, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_array, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_array_of_objects, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_collection, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_retval, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_nullable_int, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_nullable_uint, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_nullable_long, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_nullable_float, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_nullable_double, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_nullable_string, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_nullable_boolean, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_nullable_char, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_nullable_array, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_object_or_scalar, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_static_object_or_scalar, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
