
extern zend_class_entry *typehints_args_ce;

ZEPHIR_INIT_CLASS(TypeHints_Args);

PHP_METHOD(TypeHints_Args, args);

ZEND_BEGIN_ARG_INFO_EX(arginfo_typehints_args_args, 0, 0, 7)
	ZEND_ARG_INFO(0, _var)
	ZEND_ARG_INFO(0, _string)
	ZEND_ARG_INFO(0, _bool)
	ZEND_ARG_INFO(0, _int)
	ZEND_ARG_INFO(0, _long)
	ZEND_ARG_INFO(0, _double)
	ZEND_ARG_OBJ_INFO(0, _args, TypeHints\\Args, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(typehints_args_method_entry) {
	PHP_ME(TypeHints_Args, args, arginfo_typehints_args_args, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
