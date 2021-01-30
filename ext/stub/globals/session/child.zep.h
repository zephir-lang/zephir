
extern zend_class_entry *stub_globals_session_child_ce;

ZEPHIR_INIT_CLASS(Stub_Globals_Session_Child);

PHP_METHOD(Stub_Globals_Session_Child, destroy);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_globals_session_child_destroy, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_globals_session_child_method_entry) {
	PHP_ME(Stub_Globals_Session_Child, destroy, arginfo_stub_globals_session_child_destroy, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
