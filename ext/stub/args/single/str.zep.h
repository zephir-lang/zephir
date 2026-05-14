
extern zend_class_entry *stub_args_single_str_ce;

ZEPHIR_INIT_CLASS(Stub_Args_Single_Str);

PHP_METHOD(Stub_Args_Single_Str, argString);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_args_single_str_argstring, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_args_single_str_method_entry) {
	PHP_ME(Stub_Args_Single_Str, argString, arginfo_stub_args_single_str_argstring, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
