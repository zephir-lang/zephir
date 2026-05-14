
extern zend_class_entry *stub_args_single_stroptional_ce;

ZEPHIR_INIT_CLASS(Stub_Args_Single_StrOptional);

PHP_METHOD(Stub_Args_Single_StrOptional, argStringDefault);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_args_single_stroptional_argstringdefault, 0, 0, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, param, IS_STRING, 0, "'test string'")
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_args_single_stroptional_method_entry) {
	PHP_ME(Stub_Args_Single_StrOptional, argStringDefault, arginfo_stub_args_single_stroptional_argstringdefault, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
