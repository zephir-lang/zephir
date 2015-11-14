
extern zend_class_entry *test_unsettest_ce;

ZEPHIR_INIT_CLASS(Test_Unsettest);

PHP_METHOD(Test_Unsettest, getProperty);
PHP_METHOD(Test_Unsettest, has);
PHP_METHOD(Test_Unsettest, addValueToProperty);
PHP_METHOD(Test_Unsettest, testUnsetValueFromProperty);
PHP_METHOD(Test_Unsettest, testUnsetFromArray);
PHP_METHOD(Test_Unsettest, testUnsetFromArrayByIndexVar);
PHP_METHOD(Test_Unsettest, testUnsetProperty);
PHP_METHOD(Test_Unsettest, testStdClassUnset);
PHP_METHOD(Test_Unsettest, testUnsetTypedArray);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_unsettest_has, 0, 0, 1)
	ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_unsettest_addvaluetoproperty, 0, 0, 2)
	ZEND_ARG_INFO(0, key)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_unsettest_testunsetvaluefromproperty, 0, 0, 1)
	ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_unsettest_testunsetfromarray, 0, 0, 1)
	ZEND_ARG_INFO(0, arrayParameter)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_unsettest_testunsetfromarraybyindexvar, 0, 0, 2)
	ZEND_ARG_INFO(0, arrayParameter)
	ZEND_ARG_INFO(0, index)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_unsettest_testunsettypedarray, 0, 0, 2)
	ZEND_ARG_ARRAY_INFO(0, arr, 0)
	ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_unsettest_method_entry) {
	PHP_ME(Test_Unsettest, getProperty, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Unsettest, has, arginfo_test_unsettest_has, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Unsettest, addValueToProperty, arginfo_test_unsettest_addvaluetoproperty, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Unsettest, testUnsetValueFromProperty, arginfo_test_unsettest_testunsetvaluefromproperty, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Unsettest, testUnsetFromArray, arginfo_test_unsettest_testunsetfromarray, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Unsettest, testUnsetFromArrayByIndexVar, arginfo_test_unsettest_testunsetfromarraybyindexvar, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Unsettest, testUnsetProperty, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Unsettest, testStdClassUnset, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Unsettest, testUnsetTypedArray, arginfo_test_unsettest_testunsettypedarray, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
