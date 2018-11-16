
extern zend_class_entry *test_globals_session_child_ce;

ZEPHIR_INIT_CLASS(Test_Globals_Session_Child);

PHP_METHOD(Test_Globals_Session_Child, destroy);

ZEPHIR_INIT_FUNCS(test_globals_session_child_method_entry) {
	PHP_ME(Test_Globals_Session_Child, destroy, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
