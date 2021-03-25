
extern zend_class_entry *stub_logical_ce;

ZEPHIR_INIT_CLASS(Stub_Logical);

PHP_METHOD(Stub_Logical, testAnd1);
PHP_METHOD(Stub_Logical, testAnd2);
PHP_METHOD(Stub_Logical, testAnd3);
PHP_METHOD(Stub_Logical, testAnd4);
PHP_METHOD(Stub_Logical, testAnd9);
PHP_METHOD(Stub_Logical, testOr1);
PHP_METHOD(Stub_Logical, testOr2);
PHP_METHOD(Stub_Logical, testMixed1);
PHP_METHOD(Stub_Logical, testMixed2);
PHP_METHOD(Stub_Logical, testMixed3);
PHP_METHOD(Stub_Logical, testMixed4);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_logical_testand1, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_logical_testand2, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_logical_testand3, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_logical_testand4, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_logical_testand9, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_logical_testor1, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_logical_testor2, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_logical_testmixed1, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_logical_testmixed2, 0, 0, 2)
	ZEND_ARG_INFO(0, match)
	ZEND_ARG_INFO(0, minLength)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_logical_testmixed3, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_logical_testmixed4, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, a, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, b, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_logical_method_entry) {
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Logical, testAnd1, arginfo_stub_logical_testand1, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Logical, testAnd1, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Logical, testAnd2, arginfo_stub_logical_testand2, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Logical, testAnd2, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Logical, testAnd3, arginfo_stub_logical_testand3, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Logical, testAnd3, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Logical, testAnd4, arginfo_stub_logical_testand4, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Logical, testAnd4, NULL, ZEND_ACC_PUBLIC)
#endif
	PHP_ME(Stub_Logical, testAnd9, arginfo_stub_logical_testand9, ZEND_ACC_PUBLIC)
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Logical, testOr1, arginfo_stub_logical_testor1, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Logical, testOr1, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Logical, testOr2, arginfo_stub_logical_testor2, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Logical, testOr2, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Logical, testMixed1, arginfo_stub_logical_testmixed1, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Logical, testMixed1, NULL, ZEND_ACC_PUBLIC)
#endif
	PHP_ME(Stub_Logical, testMixed2, arginfo_stub_logical_testmixed2, ZEND_ACC_PUBLIC)
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Logical, testMixed3, arginfo_stub_logical_testmixed3, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Logical, testMixed3, NULL, ZEND_ACC_PUBLIC)
#endif
	PHP_ME(Stub_Logical, testMixed4, arginfo_stub_logical_testmixed4, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
