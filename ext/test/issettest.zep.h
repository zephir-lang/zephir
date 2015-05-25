
extern zend_class_entry *test_issettest_ce;

ZEPHIR_INIT_CLASS(Test_IssetTest);

PHP_METHOD(Test_IssetTest, testIssetArray1);
PHP_METHOD(Test_IssetTest, testIssetArray2);
PHP_METHOD(Test_IssetTest, testIssetArray3);
PHP_METHOD(Test_IssetTest, testIssetArray4);
PHP_METHOD(Test_IssetTest, testIssetArray5);
PHP_METHOD(Test_IssetTest, testIssetProperty1);
PHP_METHOD(Test_IssetTest, testIssetProperty2);
PHP_METHOD(Test_IssetTest, testIssetProperty3);
PHP_METHOD(Test_IssetTest, testIssetDynamicProperty1);
PHP_METHOD(Test_IssetTest, testIssetDynamicProperty2);
PHP_METHOD(Test_IssetTest, __construct);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_issettest_testissetarray1, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_issettest_testissetarray2, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_issettest_testissetarray3, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_issettest_testissetarray4, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_issettest_testissetarray5, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_issettest_testissetproperty1, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_issettest_testissetproperty2, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_issettest_testissetproperty3, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_issettest_testissetdynamicproperty2, 0, 0, 1)
	ZEND_ARG_INFO(0, inp)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_issettest_method_entry) {
	PHP_ME(Test_IssetTest, testIssetArray1, arginfo_test_issettest_testissetarray1, ZEND_ACC_PUBLIC)
	PHP_ME(Test_IssetTest, testIssetArray2, arginfo_test_issettest_testissetarray2, ZEND_ACC_PUBLIC)
	PHP_ME(Test_IssetTest, testIssetArray3, arginfo_test_issettest_testissetarray3, ZEND_ACC_PUBLIC)
	PHP_ME(Test_IssetTest, testIssetArray4, arginfo_test_issettest_testissetarray4, ZEND_ACC_PUBLIC)
	PHP_ME(Test_IssetTest, testIssetArray5, arginfo_test_issettest_testissetarray5, ZEND_ACC_PUBLIC)
	PHP_ME(Test_IssetTest, testIssetProperty1, arginfo_test_issettest_testissetproperty1, ZEND_ACC_PUBLIC)
	PHP_ME(Test_IssetTest, testIssetProperty2, arginfo_test_issettest_testissetproperty2, ZEND_ACC_PUBLIC)
	PHP_ME(Test_IssetTest, testIssetProperty3, arginfo_test_issettest_testissetproperty3, ZEND_ACC_PUBLIC)
	PHP_ME(Test_IssetTest, testIssetDynamicProperty1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_IssetTest, testIssetDynamicProperty2, arginfo_test_issettest_testissetdynamicproperty2, ZEND_ACC_PUBLIC)
	PHP_ME(Test_IssetTest, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_FE_END
};
