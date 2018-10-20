
extern zend_class_entry *typehints_retval_ce;

ZEPHIR_INIT_CLASS(TypeHints_RetVal);

PHP_METHOD(TypeHints_RetVal, getMyVar);
PHP_METHOD(TypeHints_RetVal, getMyString);
PHP_METHOD(TypeHints_RetVal, retval_var_var);
PHP_METHOD(TypeHints_RetVal, retval_var_var_builit_1);
PHP_METHOD(TypeHints_RetVal, retval_var_var_builit_2);
PHP_METHOD(TypeHints_RetVal, retval_var);
PHP_METHOD(TypeHints_RetVal, retval_string);
PHP_METHOD(TypeHints_RetVal, retval_boolean);
PHP_METHOD(TypeHints_RetVal, retval_int);
PHP_METHOD(TypeHints_RetVal, retval_long);
PHP_METHOD(TypeHints_RetVal, retval_double);
PHP_METHOD(TypeHints_RetVal, retval_retval);

#ifdef ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_typehints_retval_getmystring, 0, 0, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_typehints_retval_getmystring, 0, 0, IS_STRING, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#ifdef ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_typehints_retval_retval_string, 0, 0, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_typehints_retval_retval_string, 0, 0, IS_STRING, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#ifdef ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_typehints_retval_retval_boolean, 0, 0, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_typehints_retval_retval_boolean, 0, 0, _IS_BOOL, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#ifdef ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_typehints_retval_retval_int, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_typehints_retval_retval_int, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#ifdef ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_typehints_retval_retval_long, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_typehints_retval_retval_long, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#ifdef ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_typehints_retval_retval_double, 0, 0, IS_DOUBLE, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_typehints_retval_retval_double, 0, 0, IS_DOUBLE, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#ifdef ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX
ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_typehints_retval_retval_retval, 0, 0, TypeHints\\RetVal, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_typehints_retval_retval_retval, 0, 0, NULL, "TypeHints\\RetVal", 0)
#endif
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(typehints_retval_method_entry) {
	PHP_ME(TypeHints_RetVal, getMyVar, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, getMyString, arginfo_typehints_retval_getmystring, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_var_var, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_var_var_builit_1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_var_var_builit_2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_var, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_string, arginfo_typehints_retval_retval_string, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_boolean, arginfo_typehints_retval_retval_boolean, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_int, arginfo_typehints_retval_retval_int, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_long, arginfo_typehints_retval_retval_long, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_double, arginfo_typehints_retval_retval_double, ZEND_ACC_PUBLIC)
	PHP_ME(TypeHints_RetVal, retval_retval, arginfo_typehints_retval_retval_retval, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
