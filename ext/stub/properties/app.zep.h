
extern zend_class_entry *stub_properties_app_ce;

ZEPHIR_INIT_CLASS(Stub_Properties_App);

PHP_METHOD(Stub_Properties_App, getInstance);
PHP_METHOD(Stub_Properties_App, __construct);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_properties_app_getinstance, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_properties_app___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_properties_app_method_entry) {
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Properties_App, getInstance, arginfo_stub_properties_app_getinstance, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Properties_App, getInstance, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Properties_App, __construct, arginfo_stub_properties_app___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
#else
	PHP_ME(Stub_Properties_App, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
#endif
	PHP_FE_END
};
