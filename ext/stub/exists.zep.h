
extern zend_class_entry *stub_exists_ce;

ZEPHIR_INIT_CLASS(Stub_Exists);

PHP_METHOD(Stub_Exists, testClassExists);
PHP_METHOD(Stub_Exists, testInterfaceExists);
PHP_METHOD(Stub_Exists, testMethodExists);
PHP_METHOD(Stub_Exists, testFileExists);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_exists_testclassexists, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, className)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, autoload, _IS_BOOL, 0, "true")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_exists_testinterfaceexists, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, interfaceName)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, autoload, _IS_BOOL, 0, "true")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_exists_testmethodexists, 0, 2, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, obj)
	ZEND_ARG_INFO(0, methodName)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_exists_testfileexists, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, fileName)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_exists_method_entry) {
	PHP_ME(Stub_Exists, testClassExists, arginfo_stub_exists_testclassexists, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Exists, testInterfaceExists, arginfo_stub_exists_testinterfaceexists, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Exists, testMethodExists, arginfo_stub_exists_testmethodexists, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Exists, testFileExists, arginfo_stub_exists_testfileexists, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
