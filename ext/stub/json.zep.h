
extern zend_class_entry *stub_json_ce;

ZEPHIR_INIT_CLASS(Stub_Json);

PHP_METHOD(Stub_Json, testEncodeObject);
PHP_METHOD(Stub_Json, testEncodeArray);
PHP_METHOD(Stub_Json, testEncodeOptions);
PHP_METHOD(Stub_Json, testDecodeObject);
PHP_METHOD(Stub_Json, testDecodeObject2);
PHP_METHOD(Stub_Json, testDecodeArray);

ZEPHIR_INIT_FUNCS(stub_json_method_entry) {
	PHP_ME(Stub_Json, testEncodeObject, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Json, testEncodeArray, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Json, testEncodeOptions, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Json, testDecodeObject, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Json, testDecodeObject2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Json, testDecodeArray, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
