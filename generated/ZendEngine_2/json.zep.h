
extern zend_class_entry *test_json_ce;

ZEPHIR_INIT_CLASS(Test_Json);

PHP_METHOD(Test_Json, testEncodeObject);
PHP_METHOD(Test_Json, testEncodeArray);
PHP_METHOD(Test_Json, testEncodeOptions);
PHP_METHOD(Test_Json, testDecodeObject);
PHP_METHOD(Test_Json, testDecodeObject2);
PHP_METHOD(Test_Json, testDecodeArray);

ZEPHIR_INIT_FUNCS(test_json_method_entry) {
	PHP_ME(Test_Json, testEncodeObject, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Json, testEncodeArray, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Json, testEncodeOptions, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Json, testDecodeObject, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Json, testDecodeObject2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Json, testDecodeArray, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
