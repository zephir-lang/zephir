
extern zend_class_entry *stub_args_single_strmixed_ce;

ZEPHIR_INIT_CLASS(Stub_Args_Single_StrMixed);

PHP_METHOD(Stub_Args_Single_StrMixed, argStringAndInt);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_args_single_strmixed_argstringandint, 0, 2, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, position, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_args_single_strmixed_method_entry) {
	PHP_ME(Stub_Args_Single_StrMixed, argStringAndInt, arginfo_stub_args_single_strmixed_argstringandint, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
