
extern zend_class_entry *stub_invokes_invokeprotected_ce;

ZEPHIR_INIT_CLASS(Stub_Invokes_InvokeProtected);

PHP_METHOD(Stub_Invokes_InvokeProtected, __construct);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_invokes_invokeprotected___construct, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, text, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_invokes_invokeprotected_method_entry) {
	PHP_ME(Stub_Invokes_InvokeProtected, __construct, arginfo_stub_invokes_invokeprotected___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_FE_END
};
