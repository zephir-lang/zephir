
extern zend_class_entry *stub_exitdie_ce;

ZEPHIR_INIT_CLASS(Stub_ExitDie);

PHP_METHOD(Stub_ExitDie, testExit);
PHP_METHOD(Stub_ExitDie, testDie);

#if PHP_VERSION_ID >= 70100
#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_exitdie_testexit, 0, 0, IS_VOID, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_exitdie_testexit, 0, 0, IS_VOID, NULL, 0)
#endif
#else
ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_exitdie_testexit, 0, 0, 0)
#define arginfo_stub_exitdie_testexit NULL
#endif

	ZEND_ARG_INFO(0, param)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70100
#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_exitdie_testdie, 0, 0, IS_VOID, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_exitdie_testdie, 0, 0, IS_VOID, NULL, 0)
#endif
#else
ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_exitdie_testdie, 0, 0, 0)
#define arginfo_stub_exitdie_testdie NULL
#endif

	ZEND_ARG_INFO(0, param)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_exitdie_method_entry) {
	PHP_ME(Stub_ExitDie, testExit, arginfo_stub_exitdie_testexit, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ExitDie, testDie, arginfo_stub_exitdie_testdie, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
