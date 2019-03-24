
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
PHP_METHOD(Test_NativeArray, testArrayKeys);
PHP_METHOD(Test_NativeArray, testImplodeArray);
PHP_METHOD(Test_NativeArray, issue110);
PHP_METHOD(Test_NativeArray, issue264);
PHP_METHOD(Test_NativeArray, issue743a);
PHP_METHOD(Test_NativeArray, issue743b);
PHP_METHOD(Test_NativeArray, issue743c);
PHP_METHOD(Test_NativeArray, issue709);
PHP_METHOD(Test_NativeArray, Issue1140);
PHP_METHOD(Test_NativeArray, Issue1159);
zend_object *zephir_init_properties_Test_NativeArray(zend_class_entry *class_type TSRMLS_DC);

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray1, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray1, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray2, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray2, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray3, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray3, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray4, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray4, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray5, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray5, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray6, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray6, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray7, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray7, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray8, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray8, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray9, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray9, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray10, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray10, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray11, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray11, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray12, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray12, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray13, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray13, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray14, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray14, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray15, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray15, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray16, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray16, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray17, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray17, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray18, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray18, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray19, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray19, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray20, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray20, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray21, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray21, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray22, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray22, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray23, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray23, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray24, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray24, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray25, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray25, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray26, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray26, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray27, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray27, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray28, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray28, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray29, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray29, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray30, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarray30, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarrayaccess1, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarrayaccess1, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarrayaccess2, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarrayaccess2, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarrayaccess3, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarrayaccess3, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarrayaccess4, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarrayaccess4, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarrayaccess5, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarrayaccess5, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarrayaccess6, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarrayaccess6, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarraymultipleaccess1, 0, 0, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarraymultipleaccess1, 0, 0, IS_STRING, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarraymultipleaccess2, 0, 0, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarraymultipleaccess2, 0, 0, IS_STRING, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarraymultipleaccess3, 0, 0, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarraymultipleaccess3, 0, 0, IS_STRING, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarraymultipleaccess4, 0, 0, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarraymultipleaccess4, 0, 0, IS_STRING, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarraymultipleaccess5, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarraymultipleaccess5, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarrayupdate1, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarrayupdate1, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarrayupdate2, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarrayupdate2, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarrayupdate3, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarrayupdate3, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarrayupdate4, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarrayupdate4, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarrayupdate5, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarrayupdate5, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarrayappend1, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarrayappend1, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarrayappend2, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarrayappend2, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testmultiplearrayupdate1, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testmultiplearrayupdate1, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testmultiplearrayupdate2, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testmultiplearrayupdate2, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testmultiplearrayupdate3, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testmultiplearrayupdate3, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testmultiplearrayupdate4, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testmultiplearrayupdate4, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testmultiplearrayupdate5, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testmultiplearrayupdate5, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testmultiplearrayupdate6, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testmultiplearrayupdate6, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testmultiplearrayupdate7, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testmultiplearrayupdate7, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testmultiplearrayupdate8, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testmultiplearrayupdate8, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testmultiplearrayupdate9, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testmultiplearrayupdate9, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testmultiplearrayupdate10, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testmultiplearrayupdate10, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testmultiplearrayupdate11, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testmultiplearrayupdate11, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testmultiplearrayupdate12, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testmultiplearrayupdate12, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testmultiplearrayupdate13, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testmultiplearrayupdate13, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testmultiplearrayappend1, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testmultiplearrayappend1, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testmultiplearrayappend2, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testmultiplearrayappend2, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testmultiplearrayappend3, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testmultiplearrayappend3, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarraykeys, 0, 1, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testarraykeys, 0, 1, IS_ARRAY, NULL, 0)
#endif
	ZEND_ARG_INFO(0, param)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testimplodearray, 0, 1, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_testimplodearray, 0, 1, IS_STRING, NULL, 0)
#endif
	ZEND_ARG_INFO(0, param)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_issue110, 0, 0, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_issue110, 0, 0, IS_STRING, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_issue264, 0, 1, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_issue264, 0, 1, _IS_BOOL, NULL, 0)
#endif
	ZEND_ARG_ARRAY_INFO(0, tokens, 0)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_issue743a, 0, 1, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_issue743a, 0, 1, IS_ARRAY, NULL, 0)
#endif
	ZEND_ARG_ARRAY_INFO(0, current743a, 0)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_issue743b, 0, 1, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_issue743b, 0, 1, IS_ARRAY, NULL, 0)
#endif
	ZEND_ARG_ARRAY_INFO(0, current, 0)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_issue743c, 0, 1, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_issue743c, 0, 1, IS_ARRAY, NULL, 0)
#endif
	ZEND_ARG_ARRAY_INFO(0, current, 0)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_issue709, 0, 0, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_nativearray_issue709, 0, 0, _IS_BOOL, NULL, 0)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_nativearray_issue1140, 0, 0, 2)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, prefix, IS_STRING, 0)
#else
	ZEND_ARG_INFO(0, prefix)
#endif
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, baseDir, IS_STRING, 0)
#else
	ZEND_ARG_INFO(0, baseDir)
#endif
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_nativearray_method_entry) {
	PHP_ME(Test_NativeArray, testArray1, arginfo_test_nativearray_testarray1, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray2, arginfo_test_nativearray_testarray2, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray3, arginfo_test_nativearray_testarray3, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray4, arginfo_test_nativearray_testarray4, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray5, arginfo_test_nativearray_testarray5, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray6, arginfo_test_nativearray_testarray6, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray7, arginfo_test_nativearray_testarray7, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray8, arginfo_test_nativearray_testarray8, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray9, arginfo_test_nativearray_testarray9, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray10, arginfo_test_nativearray_testarray10, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray11, arginfo_test_nativearray_testarray11, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray12, arginfo_test_nativearray_testarray12, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray13, arginfo_test_nativearray_testarray13, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray14, arginfo_test_nativearray_testarray14, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray15, arginfo_test_nativearray_testarray15, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray16, arginfo_test_nativearray_testarray16, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray17, arginfo_test_nativearray_testarray17, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray18, arginfo_test_nativearray_testarray18, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray19, arginfo_test_nativearray_testarray19, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray20, arginfo_test_nativearray_testarray20, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray21, arginfo_test_nativearray_testarray21, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray22, arginfo_test_nativearray_testarray22, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray23, arginfo_test_nativearray_testarray23, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray24, arginfo_test_nativearray_testarray24, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray25, arginfo_test_nativearray_testarray25, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray26, arginfo_test_nativearray_testarray26, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray27, arginfo_test_nativearray_testarray27, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray28, arginfo_test_nativearray_testarray28, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray29, arginfo_test_nativearray_testarray29, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArray30, arginfo_test_nativearray_testarray30, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArrayAccess1, arginfo_test_nativearray_testarrayaccess1, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArrayAccess2, arginfo_test_nativearray_testarrayaccess2, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArrayAccess3, arginfo_test_nativearray_testarrayaccess3, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArrayAccess4, arginfo_test_nativearray_testarrayaccess4, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArrayAccess5, arginfo_test_nativearray_testarrayaccess5, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArrayAccess6, arginfo_test_nativearray_testarrayaccess6, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArrayMultipleAccess1, arginfo_test_nativearray_testarraymultipleaccess1, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArrayMultipleAccess2, arginfo_test_nativearray_testarraymultipleaccess2, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArrayMultipleAccess3, arginfo_test_nativearray_testarraymultipleaccess3, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArrayMultipleAccess4, arginfo_test_nativearray_testarraymultipleaccess4, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArrayMultipleAccess5, arginfo_test_nativearray_testarraymultipleaccess5, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArrayUpdate1, arginfo_test_nativearray_testarrayupdate1, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArrayUpdate2, arginfo_test_nativearray_testarrayupdate2, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArrayUpdate3, arginfo_test_nativearray_testarrayupdate3, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArrayUpdate4, arginfo_test_nativearray_testarrayupdate4, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArrayUpdate5, arginfo_test_nativearray_testarrayupdate5, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArrayAppend1, arginfo_test_nativearray_testarrayappend1, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArrayAppend2, arginfo_test_nativearray_testarrayappend2, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testMultipleArrayUpdate1, arginfo_test_nativearray_testmultiplearrayupdate1, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testMultipleArrayUpdate2, arginfo_test_nativearray_testmultiplearrayupdate2, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testMultipleArrayUpdate3, arginfo_test_nativearray_testmultiplearrayupdate3, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testMultipleArrayUpdate4, arginfo_test_nativearray_testmultiplearrayupdate4, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testMultipleArrayUpdate5, arginfo_test_nativearray_testmultiplearrayupdate5, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testMultipleArrayUpdate6, arginfo_test_nativearray_testmultiplearrayupdate6, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testMultipleArrayUpdate7, arginfo_test_nativearray_testmultiplearrayupdate7, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testMultipleArrayUpdate8, arginfo_test_nativearray_testmultiplearrayupdate8, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testMultipleArrayUpdate9, arginfo_test_nativearray_testmultiplearrayupdate9, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testMultipleArrayUpdate10, arginfo_test_nativearray_testmultiplearrayupdate10, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testMultipleArrayUpdate11, arginfo_test_nativearray_testmultiplearrayupdate11, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testMultipleArrayUpdate12, arginfo_test_nativearray_testmultiplearrayupdate12, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testMultipleArrayUpdate13, arginfo_test_nativearray_testmultiplearrayupdate13, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testMultipleArrayAppend1, arginfo_test_nativearray_testmultiplearrayappend1, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testMultipleArrayAppend2, arginfo_test_nativearray_testmultiplearrayappend2, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testMultipleArrayAppend3, arginfo_test_nativearray_testmultiplearrayappend3, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testArrayKeys, arginfo_test_nativearray_testarraykeys, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, testImplodeArray, arginfo_test_nativearray_testimplodearray, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, issue110, arginfo_test_nativearray_issue110, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, issue264, arginfo_test_nativearray_issue264, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, issue743a, arginfo_test_nativearray_issue743a, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, issue743b, arginfo_test_nativearray_issue743b, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, issue743c, arginfo_test_nativearray_issue743c, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, issue709, arginfo_test_nativearray_issue709, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, Issue1140, arginfo_test_nativearray_issue1140, ZEND_ACC_PUBLIC)
	PHP_ME(Test_NativeArray, Issue1159, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
