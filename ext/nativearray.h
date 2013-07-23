
extern zend_class_entry *test_nativearray_ce;

ZEPHIR_INIT_CLASS(Test_NativeArray);

PHP_METHOD(Test_NativeArray, testArray1);
PHP_METHOD(Test_NativeArray, testArray2);
PHP_METHOD(Test_NativeArray, testArray3);
PHP_METHOD(Test_NativeArray, testArray4);

ZEPHIR_INIT_FUNCS(test_nativearray_method_entry) {
	PHP_ME(Test_NativeArray, testArray1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray4, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
