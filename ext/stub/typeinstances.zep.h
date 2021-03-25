
extern zend_class_entry *stub_typeinstances_ce;

ZEPHIR_INIT_CLASS(Stub_TypeInstances);

PHP_METHOD(Stub_TypeInstances, testInstanceOfString1);
PHP_METHOD(Stub_TypeInstances, testInstanceOfString2);
PHP_METHOD(Stub_TypeInstances, testInstanceOfString3);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_typeinstances_testinstanceofstring1, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_typeinstances_testinstanceofstring2, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_typeinstances_testinstanceofstring3, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_typeinstances_method_entry) {
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_TypeInstances, testInstanceOfString1, arginfo_stub_typeinstances_testinstanceofstring1, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_TypeInstances, testInstanceOfString1, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_TypeInstances, testInstanceOfString2, arginfo_stub_typeinstances_testinstanceofstring2, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_TypeInstances, testInstanceOfString2, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_TypeInstances, testInstanceOfString3, arginfo_stub_typeinstances_testinstanceofstring3, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_TypeInstances, testInstanceOfString3, NULL, ZEND_ACC_PUBLIC)
#endif
	PHP_FE_END
};
