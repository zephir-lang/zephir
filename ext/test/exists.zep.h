
extern zend_class_entry *test_exists_ce;

ZEPHIR_INIT_CLASS(Test_Exists);

PHP_METHOD(Test_Exists, testClassExists);
PHP_METHOD(Test_Exists, testInterfaceExists);
PHP_METHOD(Test_Exists, testMethodExists);
PHP_METHOD(Test_Exists, testFileExists);

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_exists_testclassexists, 0, 1, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_exists_testclassexists, 0, 1, _IS_BOOL, NULL, 0)
#endif
	ZEND_ARG_INFO(0, className)
	ZEND_ARG_TYPE_INFO(0, autoload, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_exists_testinterfaceexists, 0, 1, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_exists_testinterfaceexists, 0, 1, _IS_BOOL, NULL, 0)
#endif
	ZEND_ARG_INFO(0, interfaceName)
	ZEND_ARG_TYPE_INFO(0, autoload, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_exists_testmethodexists, 0, 2, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_exists_testmethodexists, 0, 2, _IS_BOOL, NULL, 0)
#endif
	ZEND_ARG_INFO(0, obj)
	ZEND_ARG_INFO(0, methodName)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_exists_testfileexists, 0, 1, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_exists_testfileexists, 0, 1, _IS_BOOL, NULL, 0)
#endif
	ZEND_ARG_INFO(0, fileName)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_exists_method_entry) {
	PHP_ME(Test_Exists, testClassExists, arginfo_test_exists_testclassexists, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Exists, testInterfaceExists, arginfo_test_exists_testinterfaceexists, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Exists, testMethodExists, arginfo_test_exists_testmethodexists, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Exists, testFileExists, arginfo_test_exists_testfileexists, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
