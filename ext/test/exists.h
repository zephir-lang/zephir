
extern zend_class_entry *test_exists_ce;

ZEPHIR_INIT_CLASS(Test_Exists);

PHP_METHOD(Test_Exists, testClassExists);
PHP_METHOD(Test_Exists, testInterfaceExists);
PHP_METHOD(Test_Exists, testMethodExists);
PHP_METHOD(Test_Exists, testFileExists);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_exists_testclassexists, 0, 0, 1)
	ZEND_ARG_INFO(0, className)
	ZEND_ARG_INFO(0, autoload)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_exists_testinterfaceexists, 0, 0, 1)
	ZEND_ARG_INFO(0, interfaceName)
	ZEND_ARG_INFO(0, autoload)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_exists_testmethodexists, 0, 0, 2)
	ZEND_ARG_INFO(0, obj)
	ZEND_ARG_INFO(0, methodName)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_exists_testfileexists, 0, 0, 1)
	ZEND_ARG_INFO(0, fileName)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_exists_method_entry) {
	PHP_ME(Test_Exists, testClassExists, arginfo_test_exists_testclassexists, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Exists, testInterfaceExists, arginfo_test_exists_testinterfaceexists, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Exists, testMethodExists, arginfo_test_exists_testmethodexists, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Exists, testFileExists, arginfo_test_exists_testfileexists, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
