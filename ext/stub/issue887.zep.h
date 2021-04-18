
extern zend_class_entry *stub_issue887_ce;

ZEPHIR_INIT_CLASS(Stub_Issue887);

PHP_METHOD(Stub_Issue887, __construct);
PHP_METHOD(Stub_Issue887, add);
PHP_METHOD(Stub_Issue887, fetch_array);
PHP_METHOD(Stub_Issue887, fetch_object);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue887___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue887_add, 0, 0, 2)
	ZEND_ARG_INFO(0, key)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue887_fetch_array, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue887_fetch_object, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue887_method_entry) {
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Issue887, __construct, arginfo_stub_issue887___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
#else
	PHP_ME(Stub_Issue887, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
#endif
	PHP_ME(Stub_Issue887, add, arginfo_stub_issue887_add, ZEND_ACC_PUBLIC)
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Issue887, fetch_array, arginfo_stub_issue887_fetch_array, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Issue887, fetch_array, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Issue887, fetch_object, arginfo_stub_issue887_fetch_object, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Issue887, fetch_object, NULL, ZEND_ACC_PUBLIC)
#endif
	PHP_FE_END
};
