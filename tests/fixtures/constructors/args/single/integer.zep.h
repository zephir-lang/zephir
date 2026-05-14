
extern zend_class_entry *stub_args_single_integer_ce;

ZEPHIR_INIT_CLASS(Stub_Args_Single_Integer);

PHP_METHOD(Stub_Args_Single_Integer, argInt);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_args_single_integer_argint, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, i, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_args_single_integer_method_entry) {
	PHP_ME(Stub_Args_Single_Integer, argInt, arginfo_stub_args_single_integer_argint, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
