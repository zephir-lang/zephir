
extern zend_class_entry *test_nativearray_ce;

ZEPHIR_INIT_CLASS(Test_NativeArray);

PHP_METHOD(Test_NativeArray, testArray1);
PHP_METHOD(Test_NativeArray, testArray2);
PHP_METHOD(Test_NativeArray, testArray3);
PHP_METHOD(Test_NativeArray, testArray4);
PHP_METHOD(Test_NativeArray, testArray5);
PHP_METHOD(Test_NativeArray, testArray6);
PHP_METHOD(Test_NativeArray, testArray7);

ZEPHIR_INIT_FUNCS(test_nativearray_method_entry) {
	PHP_ME(Test_NativeArray, testArray1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray4, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray5, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray6, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray7, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
