
extern zend_class_entry *stub_issue2635_caller_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2635_Caller);

PHP_METHOD(Stub_Issue2635_Caller, call);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2635_caller_call, 0, 1, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0, obj, Stub\\Issue2635\\Outer, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2635_caller_method_entry) {
	PHP_ME(Stub_Issue2635_Caller, call, arginfo_stub_issue2635_caller_call, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
