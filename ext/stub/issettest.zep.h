
extern zend_class_entry *stub_issettest_ce;

ZEPHIR_INIT_CLASS(Stub_IssetTest);

PHP_METHOD(Stub_IssetTest, testIssetArray1);
PHP_METHOD(Stub_IssetTest, testIssetArray2);
PHP_METHOD(Stub_IssetTest, testIssetArray3);
PHP_METHOD(Stub_IssetTest, testIssetArray4);
PHP_METHOD(Stub_IssetTest, testIssetArray5);
PHP_METHOD(Stub_IssetTest, testIssetProperty1);
PHP_METHOD(Stub_IssetTest, testIssetProperty2);
PHP_METHOD(Stub_IssetTest, testIssetProperty3);
PHP_METHOD(Stub_IssetTest, testIssetDynamicProperty1);
PHP_METHOD(Stub_IssetTest, testIssetDynamicProperty2);
zend_object *zephir_init_properties_Stub_IssetTest(zend_class_entry *class_type);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issettest_testissetarray1, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issettest_testissetarray2, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, b)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issettest_testissetarray3, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, b, IS_STRING, 0)
#else
	ZEND_ARG_INFO(0, b)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issettest_testissetarray4, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issettest_testissetarray5, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issettest_testissetproperty1, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issettest_testissetproperty2, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issettest_testissetproperty3, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issettest_testissetdynamicproperty2, 0, 0, 1)
	ZEND_ARG_INFO(0, inp)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issettest_method_entry) {
	PHP_ME(Stub_IssetTest, testIssetArray1, arginfo_stub_issettest_testissetarray1, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_IssetTest, testIssetArray2, arginfo_stub_issettest_testissetarray2, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_IssetTest, testIssetArray3, arginfo_stub_issettest_testissetarray3, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_IssetTest, testIssetArray4, arginfo_stub_issettest_testissetarray4, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_IssetTest, testIssetArray5, arginfo_stub_issettest_testissetarray5, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_IssetTest, testIssetProperty1, arginfo_stub_issettest_testissetproperty1, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_IssetTest, testIssetProperty2, arginfo_stub_issettest_testissetproperty2, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_IssetTest, testIssetProperty3, arginfo_stub_issettest_testissetproperty3, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_IssetTest, testIssetDynamicProperty1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_IssetTest, testIssetDynamicProperty2, arginfo_stub_issettest_testissetdynamicproperty2, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
