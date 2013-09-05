
extern zend_class_entry *test_nativearray_ce;

ZEPHIR_INIT_CLASS(Test_NativeArray);

PHP_METHOD(Test_NativeArray, testArray1);
PHP_METHOD(Test_NativeArray, testArray2);
PHP_METHOD(Test_NativeArray, testArray3);
PHP_METHOD(Test_NativeArray, testArray4);
PHP_METHOD(Test_NativeArray, testArray5);
PHP_METHOD(Test_NativeArray, testArray6);
PHP_METHOD(Test_NativeArray, testArray7);
PHP_METHOD(Test_NativeArray, testArray8);
PHP_METHOD(Test_NativeArray, testArray9);
PHP_METHOD(Test_NativeArray, testArray10);
PHP_METHOD(Test_NativeArray, testArray11);
PHP_METHOD(Test_NativeArray, testArray12);
PHP_METHOD(Test_NativeArray, testArray13);
PHP_METHOD(Test_NativeArray, testArray14);
PHP_METHOD(Test_NativeArray, testArray15);
PHP_METHOD(Test_NativeArray, testArray16);
PHP_METHOD(Test_NativeArray, testArray17);
PHP_METHOD(Test_NativeArray, testArray18);
PHP_METHOD(Test_NativeArray, testArray19);
PHP_METHOD(Test_NativeArray, testArray20);
PHP_METHOD(Test_NativeArray, testArray21);
PHP_METHOD(Test_NativeArray, testArray22);
PHP_METHOD(Test_NativeArray, testArray23);
PHP_METHOD(Test_NativeArray, testArray24);
PHP_METHOD(Test_NativeArray, testArray25);
PHP_METHOD(Test_NativeArray, testArray26);
PHP_METHOD(Test_NativeArray, testArray27);
PHP_METHOD(Test_NativeArray, testArray28);
PHP_METHOD(Test_NativeArray, testArray29);
PHP_METHOD(Test_NativeArray, testArray30);
PHP_METHOD(Test_NativeArray, testArrayAccess1);
PHP_METHOD(Test_NativeArray, testArrayAccess2);
PHP_METHOD(Test_NativeArray, testArrayAccess3);
PHP_METHOD(Test_NativeArray, testArrayAccess4);
PHP_METHOD(Test_NativeArray, testArrayAccess5);
PHP_METHOD(Test_NativeArray, testArrayUpdate1);
PHP_METHOD(Test_NativeArray, testArrayUpdate2);
PHP_METHOD(Test_NativeArray, testArrayUpdate3);
PHP_METHOD(Test_NativeArray, testArrayUpdate4);
PHP_METHOD(Test_NativeArray, testArrayWrongUpdate1);

ZEPHIR_INIT_FUNCS(test_nativearray_method_entry) {
	PHP_ME(Test_NativeArray, testArray1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray4, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray5, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray6, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray7, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray8, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray9, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray10, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray11, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray12, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray13, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray14, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray15, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray16, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray17, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray18, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray19, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray20, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray21, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray22, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray23, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray24, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray25, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray26, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray27, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray28, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray29, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray30, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArrayAccess1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArrayAccess2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArrayAccess3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArrayAccess4, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArrayAccess5, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArrayUpdate1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArrayUpdate2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArrayUpdate3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArrayUpdate4, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArrayWrongUpdate1, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
