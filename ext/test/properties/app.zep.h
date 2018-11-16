
extern zend_class_entry *test_properties_app_ce;

ZEPHIR_INIT_CLASS(Test_Properties_App);

PHP_METHOD(Test_Properties_App, getInstance);
PHP_METHOD(Test_Properties_App, __construct);

ZEPHIR_INIT_FUNCS(test_properties_app_method_entry) {
	PHP_ME(Test_Properties_App, getInstance, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Properties_App, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_FE_END
};
