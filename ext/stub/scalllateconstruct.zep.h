
extern zend_class_entry *stub_scalllateconstruct_ce;

ZEPHIR_INIT_CLASS(Stub_ScallLateConstruct);

PHP_METHOD(Stub_ScallLateConstruct, __construct);
PHP_METHOD(Stub_ScallLateConstruct, testStaticInit);
PHP_METHOD(Stub_ScallLateConstruct, testPublicInit);
PHP_METHOD(Stub_ScallLateConstruct, varValue);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_scalllateconstruct___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_scalllateconstruct_teststaticinit, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_scalllateconstruct_testpublicinit, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_scalllateconstruct_varvalue, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_scalllateconstruct_method_entry) {
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_ScallLateConstruct, __construct, arginfo_stub_scalllateconstruct___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
#else
	PHP_ME(Stub_ScallLateConstruct, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
#endif
	PHP_ME(Stub_ScallLateConstruct, testStaticInit, arginfo_stub_scalllateconstruct_teststaticinit, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_ScallLateConstruct, testPublicInit, arginfo_stub_scalllateconstruct_testpublicinit, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_ScallLateConstruct, testPublicInit, NULL, ZEND_ACC_PUBLIC)
#endif
	PHP_ME(Stub_ScallLateConstruct, varValue, arginfo_stub_scalllateconstruct_varvalue, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
