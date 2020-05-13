
extern zend_class_entry *stub_mcallchained_ce;

ZEPHIR_INIT_CLASS(Stub_McallChained);

PHP_METHOD(Stub_McallChained, testMethod1);
PHP_METHOD(Stub_McallChained, testMethod2);
PHP_METHOD(Stub_McallChained, testMethod3);
PHP_METHOD(Stub_McallChained, testChained1);
PHP_METHOD(Stub_McallChained, testChained2);
PHP_METHOD(Stub_McallChained, testChained3);
PHP_METHOD(Stub_McallChained, testChained4);

ZEPHIR_INIT_FUNCS(stub_mcallchained_method_entry) {
	PHP_ME(Stub_McallChained, testMethod1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_McallChained, testMethod2, NULL, ZEND_ACC_PROTECTED)
	PHP_ME(Stub_McallChained, testMethod3, NULL, ZEND_ACC_PRIVATE)
	PHP_ME(Stub_McallChained, testChained1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_McallChained, testChained2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_McallChained, testChained3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_McallChained, testChained4, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
