
extern zend_class_entry *stub_echoes_ce;

ZEPHIR_INIT_CLASS(Stub_Echoes);

PHP_METHOD(Stub_Echoes, testEcho1);
PHP_METHOD(Stub_Echoes, testEcho2);
PHP_METHOD(Stub_Echoes, testEcho3);
PHP_METHOD(Stub_Echoes, testEcho4);
PHP_METHOD(Stub_Echoes, testEcho5);

ZEPHIR_INIT_FUNCS(stub_echoes_method_entry) {
	PHP_ME(Stub_Echoes, testEcho1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Echoes, testEcho2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Echoes, testEcho3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Echoes, testEcho4, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Echoes, testEcho5, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
