
extern zend_class_entry *stub_scallexternal_ce;

ZEPHIR_INIT_CLASS(Stub_ScallExternal);

PHP_METHOD(Stub_ScallExternal, testCall1);
PHP_METHOD(Stub_ScallExternal, testCall2);
PHP_METHOD(Stub_ScallExternal, testMethod3);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_scallexternal_testcall1, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_scallexternal_testcall2, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_scallexternal_testmethod3, 0, 0, 2)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, a)
#endif
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, b)
#endif
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_scallexternal_method_entry) {
	PHP_ME(Stub_ScallExternal, testCall1, arginfo_stub_scallexternal_testcall1, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ScallExternal, testCall2, arginfo_stub_scallexternal_testcall2, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ScallExternal, testMethod3, arginfo_stub_scallexternal_testmethod3, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
