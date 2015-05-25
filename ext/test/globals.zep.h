
extern zend_class_entry *test_globals_ce;

ZEPHIR_INIT_CLASS(Test_Globals);

PHP_METHOD(Test_Globals, getDefaultGlobals1);
PHP_METHOD(Test_Globals, getDefaultGlobals2);
PHP_METHOD(Test_Globals, getDefaultGlobals3);
PHP_METHOD(Test_Globals, getDefaultGlobals4);
PHP_METHOD(Test_Globals, getDefaultGlobals5);
PHP_METHOD(Test_Globals, getDefaultGlobals6);

ZEPHIR_INIT_FUNCS(test_globals_method_entry) {
	PHP_ME(Test_Globals, getDefaultGlobals1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Globals, getDefaultGlobals2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Globals, getDefaultGlobals3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Globals, getDefaultGlobals4, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Globals, getDefaultGlobals5, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Globals, getDefaultGlobals6, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
