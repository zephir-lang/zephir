
extern zend_class_entry *stub_globals_session_child_ce;

ZEPHIR_INIT_CLASS(Stub_Globals_Session_Child);

PHP_METHOD(Stub_Globals_Session_Child, destroy);

ZEPHIR_INIT_FUNCS(stub_globals_session_child_method_entry) {
	PHP_ME(Stub_Globals_Session_Child, destroy, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
