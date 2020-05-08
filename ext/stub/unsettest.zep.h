
extern zend_class_entry *stub_unsettest_ce;

ZEPHIR_INIT_CLASS(Stub_Unsettest);

PHP_METHOD(Stub_Unsettest, getProperty);
PHP_METHOD(Stub_Unsettest, has);
PHP_METHOD(Stub_Unsettest, addValueToProperty);
PHP_METHOD(Stub_Unsettest, testUnsetValueFromProperty);
PHP_METHOD(Stub_Unsettest, testUnsetFromArray);
PHP_METHOD(Stub_Unsettest, testUnsetFromArrayByIndexVar);
PHP_METHOD(Stub_Unsettest, testUnsetProperty);
PHP_METHOD(Stub_Unsettest, testStdClassUnset);
PHP_METHOD(Stub_Unsettest, testUnsetTypedArray);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_unsettest_has, 0, 0, 1)
	ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_unsettest_addvaluetoproperty, 0, 0, 2)
	ZEND_ARG_INFO(0, key)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_unsettest_testunsetvaluefromproperty, 0, 0, 1)
	ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_unsettest_testunsetfromarray, 0, 0, 1)
	ZEND_ARG_INFO(0, arrayParameter)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_unsettest_testunsetfromarraybyindexvar, 0, 0, 2)
	ZEND_ARG_INFO(0, arrayParameter)
	ZEND_ARG_INFO(0, index)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_unsettest_testunsettypedarray, 0, 0, 2)
	ZEND_ARG_ARRAY_INFO(0, arr, 0)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
#else
	ZEND_ARG_INFO(0, key)
#endif
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_unsettest_method_entry) {
	PHP_ME(Stub_Unsettest, getProperty, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Unsettest, has, arginfo_stub_unsettest_has, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Unsettest, addValueToProperty, arginfo_stub_unsettest_addvaluetoproperty, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Unsettest, testUnsetValueFromProperty, arginfo_stub_unsettest_testunsetvaluefromproperty, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Unsettest, testUnsetFromArray, arginfo_stub_unsettest_testunsetfromarray, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Unsettest, testUnsetFromArrayByIndexVar, arginfo_stub_unsettest_testunsetfromarraybyindexvar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Unsettest, testUnsetProperty, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Unsettest, testStdClassUnset, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Unsettest, testUnsetTypedArray, arginfo_stub_unsettest_testunsettypedarray, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
