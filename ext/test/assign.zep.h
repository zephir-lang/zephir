
extern zend_class_entry *test_assign_ce;

ZEPHIR_INIT_CLASS(Test_Assign);

PHP_METHOD(Test_Assign, getTestVar);
PHP_METHOD(Test_Assign, getMyArray);
PHP_METHOD(Test_Assign, testAssign1);
PHP_METHOD(Test_Assign, testAssign2);
PHP_METHOD(Test_Assign, testAssign3);
PHP_METHOD(Test_Assign, testAssign4);
PHP_METHOD(Test_Assign, testAssign5);
PHP_METHOD(Test_Assign, testAssign6);
PHP_METHOD(Test_Assign, testAssign7);
PHP_METHOD(Test_Assign, testAssign8);
PHP_METHOD(Test_Assign, testAssign9);
PHP_METHOD(Test_Assign, testAssign10);
PHP_METHOD(Test_Assign, testAssign11);
PHP_METHOD(Test_Assign, testAssign12);
PHP_METHOD(Test_Assign, testAssign13);
PHP_METHOD(Test_Assign, testAssign14);
PHP_METHOD(Test_Assign, testAssign15);
PHP_METHOD(Test_Assign, testAssign16);
PHP_METHOD(Test_Assign, testAssign17);
PHP_METHOD(Test_Assign, testAssign18);
PHP_METHOD(Test_Assign, testAssign19);
PHP_METHOD(Test_Assign, testAssign20);
PHP_METHOD(Test_Assign, testAssign21);
PHP_METHOD(Test_Assign, testAssign22);
PHP_METHOD(Test_Assign, testAssign23);
PHP_METHOD(Test_Assign, testAssign24);
PHP_METHOD(Test_Assign, testAssign25);
PHP_METHOD(Test_Assign, testAssign26);
PHP_METHOD(Test_Assign, testAssign27);
PHP_METHOD(Test_Assign, testAssign28);
PHP_METHOD(Test_Assign, testAssign29);
PHP_METHOD(Test_Assign, testAssign30);
PHP_METHOD(Test_Assign, testAssign31);
PHP_METHOD(Test_Assign, testAssign32);
PHP_METHOD(Test_Assign, testAssign33);
PHP_METHOD(Test_Assign, testAssign34);
PHP_METHOD(Test_Assign, testAssign35);
PHP_METHOD(Test_Assign, testAssign36);
PHP_METHOD(Test_Assign, testAssign37);
PHP_METHOD(Test_Assign, testAssign38);
PHP_METHOD(Test_Assign, testAssign39);
PHP_METHOD(Test_Assign, testAssign40);
PHP_METHOD(Test_Assign, testAssign41);
PHP_METHOD(Test_Assign, testAssign42);
PHP_METHOD(Test_Assign, testAssign43);
PHP_METHOD(Test_Assign, testAssign44);
PHP_METHOD(Test_Assign, testPropertyAssign1);
PHP_METHOD(Test_Assign, testPropertyAssign2);
PHP_METHOD(Test_Assign, testPropertyIncr1);
PHP_METHOD(Test_Assign, testPropertyAddAssign1);
PHP_METHOD(Test_Assign, testPropertyAddAssign2);
PHP_METHOD(Test_Assign, testPropertyAssignValuePlus1);
PHP_METHOD(Test_Assign, testPropertyDecr);
PHP_METHOD(Test_Assign, testPropertySubAssign1);
PHP_METHOD(Test_Assign, testPropertySubAssign2);
PHP_METHOD(Test_Assign, testPropertyMulAssign1);
PHP_METHOD(Test_Assign, testPropertyMulAssign2);
PHP_METHOD(Test_Assign, testPropertyAssignStringConcat);
PHP_METHOD(Test_Assign, testPropertyArray1);
PHP_METHOD(Test_Assign, testPropertyArray2);
PHP_METHOD(Test_Assign, testPropertyArray3);
PHP_METHOD(Test_Assign, testPropertyArray4);
PHP_METHOD(Test_Assign, testPropertyArray5);
PHP_METHOD(Test_Assign, testPropertyArray6);
PHP_METHOD(Test_Assign, testPropertyArray7);
PHP_METHOD(Test_Assign, testPropertyArray8);
PHP_METHOD(Test_Assign, testPropertyArray9);
PHP_METHOD(Test_Assign, testPropertyArray10);
PHP_METHOD(Test_Assign, testPropertyArray11);
PHP_METHOD(Test_Assign, testPropertyArray12);
PHP_METHOD(Test_Assign, testPropertyArray13);
PHP_METHOD(Test_Assign, testPropertyArray14);
PHP_METHOD(Test_Assign, testStaticPropertyAssign1);
PHP_METHOD(Test_Assign, testStaticPropertyAssign2);
PHP_METHOD(Test_Assign, testStaticPropertyArray1);
PHP_METHOD(Test_Assign, testStaticPropertyArray2);
PHP_METHOD(Test_Assign, testStaticPropertyArray3);
PHP_METHOD(Test_Assign, testStaticPropertyArrayAppend);
PHP_METHOD(Test_Assign, testStaticPropertyArrayMutli1);
PHP_METHOD(Test_Assign, testStaticPropertyArrayMutli2);
PHP_METHOD(Test_Assign, testStaticPropertyArrayMutli3);
PHP_METHOD(Test_Assign, testStaticPropertyArrayMulti4);
PHP_METHOD(Test_Assign, testStaticPropertyArrayAppend1);
PHP_METHOD(Test_Assign, testArrayVarAssign1);
PHP_METHOD(Test_Assign, testArrayVarAssign2);
PHP_METHOD(Test_Assign, testArrayProperty);
PHP_METHOD(Test_Assign, testGlobalVarAssign);
PHP_METHOD(Test_Assign, testConstantKeyAssign);
PHP_METHOD(Test_Assign, testArrayBoolExpressionAssign);
PHP_METHOD(Test_Assign, testAssignSuperGlobals);
PHP_METHOD(Test_Assign, testAssignSuperGlobalsSERVER);
PHP_METHOD(Test_Assign, testAssignSuperGlobalsGET);

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign1, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign1, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign2, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign2, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign3, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign3, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign4, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign4, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign5, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign5, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign6, 0, 0, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign6, 0, 0, _IS_BOOL, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign7, 0, 0, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign7, 0, 0, _IS_BOOL, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign8, 0, 0, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign8, 0, 0, _IS_BOOL, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign9, 0, 0, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign9, 0, 0, _IS_BOOL, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign10, 0, 0, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign10, 0, 0, _IS_BOOL, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign11, 0, 0, IS_DOUBLE, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign11, 0, 0, IS_DOUBLE, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign12, 0, 0, IS_DOUBLE, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign12, 0, 0, IS_DOUBLE, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign13, 0, 0, IS_DOUBLE, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign13, 0, 0, IS_DOUBLE, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign14, 0, 0, IS_DOUBLE, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign14, 0, 0, IS_DOUBLE, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign15, 0, 0, IS_DOUBLE, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign15, 0, 0, IS_DOUBLE, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign16, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign16, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign17, 0, 0, IS_DOUBLE, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign17, 0, 0, IS_DOUBLE, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign18, 0, 0, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign18, 0, 0, _IS_BOOL, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign19, 0, 0, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign19, 0, 0, _IS_BOOL, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign20, 0, 0, IS_NULL, 1)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign20, 0, 0, IS_NULL, NULL, 1)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign21, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign21, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign22, 0, 0, IS_DOUBLE, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign22, 0, 0, IS_DOUBLE, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign23, 0, 0, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign23, 0, 0, _IS_BOOL, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign24, 0, 0, IS_DOUBLE, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign24, 0, 0, IS_DOUBLE, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign25, 0, 0, IS_DOUBLE, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign25, 0, 0, IS_DOUBLE, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign26, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign26, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign27, 0, 0, IS_DOUBLE, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign27, 0, 0, IS_DOUBLE, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign28, 0, 0, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign28, 0, 0, _IS_BOOL, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign29, 0, 0, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign29, 0, 0, _IS_BOOL, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign30, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign30, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign31, 0, 0, IS_DOUBLE, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign31, 0, 0, IS_DOUBLE, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign32, 0, 0, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign32, 0, 0, _IS_BOOL, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign33, 0, 0, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign33, 0, 0, _IS_BOOL, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign34, 0, 0, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign34, 0, 0, _IS_BOOL, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign35, 0, 0, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign35, 0, 0, _IS_BOOL, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign36, 0, 0, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign36, 0, 0, _IS_BOOL, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign37, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign37, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign38, 0, 1, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign38, 0, 1, IS_ARRAY, NULL, 0)
#endif
	ZEND_ARG_INFO(0, index)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign39, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign39, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign40, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign40, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign41, 0, 1, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign41, 0, 1, IS_LONG, NULL, 0)
#endif
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, num, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, num)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign42, 0, 1, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign42, 0, 1, IS_LONG, NULL, 0)
#endif
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, num, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, num)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign43, 0, 1, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign43, 0, 1, IS_LONG, NULL, 0)
#endif
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, num, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, num)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign44, 0, 1, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_assign_testassign44, 0, 1, IS_LONG, NULL, 0)
#endif
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, num, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, num)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_assign_testpropertyarray4, 0, 0, 1)
	ZEND_ARG_INFO(0, index)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_assign_testpropertyarray5, 0, 0, 1)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, index, IS_STRING, 0)
#else
	ZEND_ARG_INFO(0, index)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_assign_testpropertyarray8, 0, 0, 1)
	ZEND_ARG_INFO(0, index)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_assign_testpropertyarray9, 0, 0, 1)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, index, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, index)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_assign_testpropertyarray10, 0, 0, 1)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, index, IS_STRING, 0)
#else
	ZEND_ARG_INFO(0, index)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_assign_testpropertyarray11, 0, 0, 1)
	ZEND_ARG_INFO(0, index)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_assign_testpropertyarray12, 0, 0, 1)
	ZEND_ARG_INFO(0, index)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_assign_testpropertyarray13, 0, 0, 1)
	ZEND_ARG_INFO(0, index)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_assign_teststaticpropertyarray3, 0, 0, 1)
	ZEND_ARG_INFO(0, index)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_assign_teststaticpropertyarraymutli3, 0, 0, 1)
	ZEND_ARG_INFO(0, index)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_assign_testarrayvarassign1, 0, 0, 2)
	ZEND_ARG_INFO(0, index)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_assign_testarrayvarassign2, 0, 0, 2)
	ZEND_ARG_INFO(0, index)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_assign_testarrayproperty, 0, 0, 2)
	ZEND_ARG_INFO(0, index)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_assign_testglobalvarassign, 0, 0, 2)
	ZEND_ARG_INFO(0, index)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_assign_method_entry) {
	PHP_ME(Test_Assign, getTestVar, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, getMyArray, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign1, arginfo_test_assign_testassign1, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign2, arginfo_test_assign_testassign2, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign3, arginfo_test_assign_testassign3, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign4, arginfo_test_assign_testassign4, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign5, arginfo_test_assign_testassign5, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign6, arginfo_test_assign_testassign6, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign7, arginfo_test_assign_testassign7, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign8, arginfo_test_assign_testassign8, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign9, arginfo_test_assign_testassign9, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign10, arginfo_test_assign_testassign10, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign11, arginfo_test_assign_testassign11, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign12, arginfo_test_assign_testassign12, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign13, arginfo_test_assign_testassign13, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign14, arginfo_test_assign_testassign14, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign15, arginfo_test_assign_testassign15, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign16, arginfo_test_assign_testassign16, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign17, arginfo_test_assign_testassign17, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign18, arginfo_test_assign_testassign18, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign19, arginfo_test_assign_testassign19, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign20, arginfo_test_assign_testassign20, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign21, arginfo_test_assign_testassign21, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign22, arginfo_test_assign_testassign22, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign23, arginfo_test_assign_testassign23, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign24, arginfo_test_assign_testassign24, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign25, arginfo_test_assign_testassign25, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign26, arginfo_test_assign_testassign26, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign27, arginfo_test_assign_testassign27, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign28, arginfo_test_assign_testassign28, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign29, arginfo_test_assign_testassign29, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign30, arginfo_test_assign_testassign30, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign31, arginfo_test_assign_testassign31, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign32, arginfo_test_assign_testassign32, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign33, arginfo_test_assign_testassign33, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign34, arginfo_test_assign_testassign34, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign35, arginfo_test_assign_testassign35, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign36, arginfo_test_assign_testassign36, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign37, arginfo_test_assign_testassign37, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign38, arginfo_test_assign_testassign38, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign39, arginfo_test_assign_testassign39, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign40, arginfo_test_assign_testassign40, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign41, arginfo_test_assign_testassign41, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign42, arginfo_test_assign_testassign42, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign43, arginfo_test_assign_testassign43, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign44, arginfo_test_assign_testassign44, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testPropertyAssign1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testPropertyAssign2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testPropertyIncr1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testPropertyAddAssign1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testPropertyAddAssign2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testPropertyAssignValuePlus1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testPropertyDecr, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testPropertySubAssign1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testPropertySubAssign2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testPropertyMulAssign1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testPropertyMulAssign2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testPropertyAssignStringConcat, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testPropertyArray1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testPropertyArray2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testPropertyArray3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testPropertyArray4, arginfo_test_assign_testpropertyarray4, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testPropertyArray5, arginfo_test_assign_testpropertyarray5, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testPropertyArray6, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testPropertyArray7, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testPropertyArray8, arginfo_test_assign_testpropertyarray8, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testPropertyArray9, arginfo_test_assign_testpropertyarray9, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testPropertyArray10, arginfo_test_assign_testpropertyarray10, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testPropertyArray11, arginfo_test_assign_testpropertyarray11, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testPropertyArray12, arginfo_test_assign_testpropertyarray12, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testPropertyArray13, arginfo_test_assign_testpropertyarray13, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testPropertyArray14, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testStaticPropertyAssign1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testStaticPropertyAssign2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testStaticPropertyArray1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testStaticPropertyArray2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testStaticPropertyArray3, arginfo_test_assign_teststaticpropertyarray3, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testStaticPropertyArrayAppend, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testStaticPropertyArrayMutli1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testStaticPropertyArrayMutli2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testStaticPropertyArrayMutli3, arginfo_test_assign_teststaticpropertyarraymutli3, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testStaticPropertyArrayMulti4, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testStaticPropertyArrayAppend1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testArrayVarAssign1, arginfo_test_assign_testarrayvarassign1, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testArrayVarAssign2, arginfo_test_assign_testarrayvarassign2, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testArrayProperty, arginfo_test_assign_testarrayproperty, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testGlobalVarAssign, arginfo_test_assign_testglobalvarassign, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testConstantKeyAssign, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testArrayBoolExpressionAssign, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssignSuperGlobals, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssignSuperGlobalsSERVER, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssignSuperGlobalsGET, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
