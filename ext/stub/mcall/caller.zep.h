
extern zend_class_entry *stub_mcall_caller_ce;

ZEPHIR_INIT_CLASS(Stub_Mcall_Caller);

PHP_METHOD(Stub_Mcall_Caller, start);
PHP_METHOD(Stub_Mcall_Caller, perform);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcall_caller_start, 0, 0, 1)
	ZEND_ARG_INFO(0, f)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_mcall_caller_perform, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_mcall_caller_method_entry) {
	PHP_ME(Stub_Mcall_Caller, start, arginfo_stub_mcall_caller_start, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Mcall_Caller, perform, arginfo_stub_mcall_caller_perform, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
