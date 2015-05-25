
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
PHP_METHOD(Test_NativeArray, testArrayAccess6);
PHP_METHOD(Test_NativeArray, testArrayMultipleAccess1);
PHP_METHOD(Test_NativeArray, testArrayMultipleAccess2);
PHP_METHOD(Test_NativeArray, testArrayMultipleAccess3);
PHP_METHOD(Test_NativeArray, testArrayMultipleAccess4);
PHP_METHOD(Test_NativeArray, testArrayMultipleAccess5);
PHP_METHOD(Test_NativeArray, testArrayUpdate1);
PHP_METHOD(Test_NativeArray, testArrayUpdate2);
PHP_METHOD(Test_NativeArray, testArrayUpdate3);
PHP_METHOD(Test_NativeArray, testArrayUpdate4);
PHP_METHOD(Test_NativeArray, testArrayUpdate5);
PHP_METHOD(Test_NativeArray, testArrayAppend1);
PHP_METHOD(Test_NativeArray, testArrayAppend2);
PHP_METHOD(Test_NativeArray, testMultipleArrayUpdate1);
PHP_METHOD(Test_NativeArray, testMultipleArrayUpdate2);
PHP_METHOD(Test_NativeArray, testMultipleArrayUpdate3);
PHP_METHOD(Test_NativeArray, testMultipleArrayUpdate4);
PHP_METHOD(Test_NativeArray, testMultipleArrayUpdate5);
PHP_METHOD(Test_NativeArray, testMultipleArrayUpdate6);
PHP_METHOD(Test_NativeArray, testMultipleArrayUpdate7);
PHP_METHOD(Test_NativeArray, testMultipleArrayUpdate8);
PHP_METHOD(Test_NativeArray, testMultipleArrayUpdate9);
PHP_METHOD(Test_NativeArray, testMultipleArrayUpdate10);
PHP_METHOD(Test_NativeArray, testMultipleArrayUpdate11);
PHP_METHOD(Test_NativeArray, testMultipleArrayUpdate12);
PHP_METHOD(Test_NativeArray, testMultipleArrayUpdate13);
PHP_METHOD(Test_NativeArray, testMultipleArrayAppend1);
PHP_METHOD(Test_NativeArray, testMultipleArrayAppend2);
PHP_METHOD(Test_NativeArray, testMultipleArrayAppend3);
PHP_METHOD(Test_NativeArray, testArrayWrongUpdate1);
PHP_METHOD(Test_NativeArray, testArrayKeys);
PHP_METHOD(Test_NativeArray, testImplodeArray);
PHP_METHOD(Test_NativeArray, issue110);
PHP_METHOD(Test_NativeArray, issue264);
PHP_METHOD(Test_NativeArray, issue743a);
PHP_METHOD(Test_NativeArray, issue743b);
PHP_METHOD(Test_NativeArray, issue743c);
PHP_METHOD(Test_NativeArray, issue709);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_nativearray_testarraykeys, 0, 0, 1)
	ZEND_ARG_INFO(0, param)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_nativearray_testimplodearray, 0, 0, 1)
	ZEND_ARG_INFO(0, param)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_nativearray_issue264, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, tokens, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_nativearray_issue743a, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, current, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_nativearray_issue743b, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, current, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_nativearray_issue743c, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, current, 0)
ZEND_END_ARG_INFO()

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
	PHP_ME(Test_NativeArray, testArrayAccess6, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArrayMultipleAccess1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArrayMultipleAccess2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArrayMultipleAccess3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArrayMultipleAccess4, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArrayMultipleAccess5, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArrayUpdate1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArrayUpdate2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArrayUpdate3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArrayUpdate4, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArrayUpdate5, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArrayAppend1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArrayAppend2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testMultipleArrayUpdate1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testMultipleArrayUpdate2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testMultipleArrayUpdate3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testMultipleArrayUpdate4, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testMultipleArrayUpdate5, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testMultipleArrayUpdate6, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testMultipleArrayUpdate7, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testMultipleArrayUpdate8, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testMultipleArrayUpdate9, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testMultipleArrayUpdate10, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testMultipleArrayUpdate11, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testMultipleArrayUpdate12, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testMultipleArrayUpdate13, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testMultipleArrayAppend1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testMultipleArrayAppend2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testMultipleArrayAppend3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArrayWrongUpdate1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArrayKeys, arginfo_test_nativearray_testarraykeys, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testImplodeArray, arginfo_test_nativearray_testimplodearray, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, issue110, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, issue264, arginfo_test_nativearray_issue264, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, issue743a, arginfo_test_nativearray_issue743a, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, issue743b, arginfo_test_nativearray_issue743b, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, issue743c, arginfo_test_nativearray_issue743c, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, issue709, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
