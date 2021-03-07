
extern zend_class_entry *stub_echoes_ce;

ZEPHIR_INIT_CLASS(Stub_Echoes);

PHP_METHOD(Stub_Echoes, testEcho1);
PHP_METHOD(Stub_Echoes, testEcho2);
PHP_METHOD(Stub_Echoes, testEcho3);
PHP_METHOD(Stub_Echoes, testEcho4);
PHP_METHOD(Stub_Echoes, testEcho5);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_echoes_testecho1, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_echoes_testecho2, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_echoes_testecho3, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_echoes_testecho4, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_echoes_testecho5, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_echoes_method_entry) {
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Echoes, testEcho1, arginfo_stub_echoes_testecho1, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Echoes, testEcho1, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Echoes, testEcho2, arginfo_stub_echoes_testecho2, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Echoes, testEcho2, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Echoes, testEcho3, arginfo_stub_echoes_testecho3, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Echoes, testEcho3, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Echoes, testEcho4, arginfo_stub_echoes_testecho4, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Echoes, testEcho4, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Echoes, testEcho5, arginfo_stub_echoes_testecho5, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Echoes, testEcho5, NULL, ZEND_ACC_PUBLIC)
#endif
	PHP_FE_END
};
