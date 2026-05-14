
extern zend_class_entry *stub_invokes_abstractinvoker_ce;

ZEPHIR_INIT_CLASS(Stub_Invokes_AbstractInvoker);

PHP_METHOD(Stub_Invokes_AbstractInvoker, __toString);
PHP_METHOD(Stub_Invokes_AbstractInvoker, __invoke);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_invokes_abstractinvoker___tostring, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_invokes_abstractinvoker___invoke, 0, 0, Stub\\Invokes\\AbstractInvoker, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_invokes_abstractinvoker_method_entry) {
PHP_ME(Stub_Invokes_AbstractInvoker, __toString, arginfo_stub_invokes_abstractinvoker___tostring, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Invokes_AbstractInvoker, __invoke, arginfo_stub_invokes_abstractinvoker___invoke, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
