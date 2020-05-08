
extern zend_class_entry *stub_trytest_ce;

ZEPHIR_INIT_CLASS(Stub_TryTest);

PHP_METHOD(Stub_TryTest, testThrow1);
PHP_METHOD(Stub_TryTest, testThrow2);
PHP_METHOD(Stub_TryTest, testTry1);
PHP_METHOD(Stub_TryTest, testTry2);
PHP_METHOD(Stub_TryTest, testTry3);
PHP_METHOD(Stub_TryTest, testTry4);
PHP_METHOD(Stub_TryTest, testTry5);
PHP_METHOD(Stub_TryTest, testTry6);
PHP_METHOD(Stub_TryTest, testTry7);
PHP_METHOD(Stub_TryTest, testTry8);
PHP_METHOD(Stub_TryTest, someMethod1);
PHP_METHOD(Stub_TryTest, someMethod2);
PHP_METHOD(Stub_TryTest, testTry9);
PHP_METHOD(Stub_TryTest, testTry10);
PHP_METHOD(Stub_TryTest, testTry11);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_trytest_testtry4, 0, 0, 1)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, a, _IS_BOOL, 0)
#else
	ZEND_ARG_INFO(0, a)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_trytest_testtry5, 0, 0, 1)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, a, _IS_BOOL, 0)
#else
	ZEND_ARG_INFO(0, a)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_trytest_testtry6, 0, 0, 1)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, a, _IS_BOOL, 0)
#else
	ZEND_ARG_INFO(0, a)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_trytest_testtry7, 0, 0, 1)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, a, _IS_BOOL, 0)
#else
	ZEND_ARG_INFO(0, a)
#endif
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_trytest_method_entry) {
	PHP_ME(Stub_TryTest, testThrow1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_TryTest, testThrow2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_TryTest, testTry1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_TryTest, testTry2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_TryTest, testTry3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_TryTest, testTry4, arginfo_stub_trytest_testtry4, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_TryTest, testTry5, arginfo_stub_trytest_testtry5, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_TryTest, testTry6, arginfo_stub_trytest_testtry6, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_TryTest, testTry7, arginfo_stub_trytest_testtry7, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_TryTest, testTry8, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_TryTest, someMethod1, NULL, ZEND_ACC_PRIVATE)
	PHP_ME(Stub_TryTest, someMethod2, NULL, ZEND_ACC_PRIVATE)
	PHP_ME(Stub_TryTest, testTry9, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_TryTest, testTry10, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_TryTest, testTry11, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
