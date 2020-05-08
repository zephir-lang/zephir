
extern zend_class_entry *stub_scalldynamic_ce;

ZEPHIR_INIT_CLASS(Stub_ScallDynamic);

PHP_METHOD(Stub_ScallDynamic, testMethod1);
PHP_METHOD(Stub_ScallDynamic, testMethod2);
PHP_METHOD(Stub_ScallDynamic, testMethod3);
PHP_METHOD(Stub_ScallDynamic, selfDynamicCall1);

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_scalldynamic_testmethod1, 0, 0, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_scalldynamic_testmethod1, 0, 0, IS_STRING, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_scalldynamic_testmethod2, 0, 0, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_scalldynamic_testmethod2, 0, 0, IS_STRING, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_scalldynamic_testmethod3, 0, 0, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_scalldynamic_testmethod3, 0, 0, IS_STRING, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_scalldynamic_selfdynamiccall1, 0, 1, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_scalldynamic_selfdynamiccall1, 0, 1, IS_STRING, NULL, 0)
#endif
	ZEND_ARG_INFO(0, methodName)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_scalldynamic_method_entry) {
	PHP_ME(Stub_ScallDynamic, testMethod1, arginfo_stub_scalldynamic_testmethod1, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ScallDynamic, testMethod2, arginfo_stub_scalldynamic_testmethod2, ZEND_ACC_STATIC|ZEND_ACC_PROTECTED)
	PHP_ME(Stub_ScallDynamic, testMethod3, arginfo_stub_scalldynamic_testmethod3, ZEND_ACC_STATIC|ZEND_ACC_PRIVATE)
	PHP_ME(Stub_ScallDynamic, selfDynamicCall1, arginfo_stub_scalldynamic_selfdynamiccall1, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_FE_END
};
