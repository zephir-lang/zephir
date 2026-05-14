
extern zend_class_entry *stub_args_single_strnullable_ce;

ZEPHIR_INIT_CLASS(Stub_Args_Single_StrNullable);

PHP_METHOD(Stub_Args_Single_StrNullable, argStringNull);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_args_single_strnullable_argstringnull, 0, 0, IS_STRING, 1)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, param, IS_STRING, 1, "null")
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_args_single_strnullable_method_entry) {
	PHP_ME(Stub_Args_Single_StrNullable, argStringNull, arginfo_stub_args_single_strnullable_argstringnull, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
