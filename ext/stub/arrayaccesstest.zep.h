
extern zend_class_entry *stub_arrayaccesstest_ce;

ZEPHIR_INIT_CLASS(Stub_ArrayAccessTest);

PHP_METHOD(Stub_ArrayAccessTest, exits);
PHP_METHOD(Stub_ArrayAccessTest, get);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_arrayaccesstest_exits, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_arrayaccesstest_get, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_arrayaccesstest_method_entry) {
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_ArrayAccessTest, exits, arginfo_stub_arrayaccesstest_exits, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
#else
	PHP_ME(Stub_ArrayAccessTest, exits, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_ArrayAccessTest, get, arginfo_stub_arrayaccesstest_get, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
#else
	PHP_ME(Stub_ArrayAccessTest, get, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
#endif
	PHP_FE_END
};
