
extern zend_class_entry *stub_properties_app_ce;

ZEPHIR_INIT_CLASS(Stub_Properties_App);

PHP_METHOD(Stub_Properties_App, getInstance);
PHP_METHOD(Stub_Properties_App, __construct);

ZEPHIR_INIT_FUNCS(stub_properties_app_method_entry) {
	PHP_ME(Stub_Properties_App, getInstance, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Properties_App, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_FE_END
};
