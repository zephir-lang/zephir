
extern zend_class_entry *test_cast_ce;

ZEPHIR_INIT_CLASS(Test_Cast);

PHP_METHOD(Test_Cast, testIntCastFromFloat);
PHP_METHOD(Test_Cast, testIntCastFromVariableFloat);
PHP_METHOD(Test_Cast, testIntCastFromBooleanTrue);
PHP_METHOD(Test_Cast, testIntCastFromBooleanFalse);
PHP_METHOD(Test_Cast, testIntCastFromVariableBooleanTrue);
PHP_METHOD(Test_Cast, testIntCastFromVariableBooleanFalse);
PHP_METHOD(Test_Cast, testIntCastFromVariableNull);
PHP_METHOD(Test_Cast, testIntCastFromStringValue);
PHP_METHOD(Test_Cast, testIntCastFromVariableString);
PHP_METHOD(Test_Cast, testIntCastFromParameterString);
PHP_METHOD(Test_Cast, testIntCastFromNull);
PHP_METHOD(Test_Cast, testIntCastFromVariableEmptyArray);
PHP_METHOD(Test_Cast, testIntCastFromEmptyArray);
PHP_METHOD(Test_Cast, testIntCastFromVariableArray);
PHP_METHOD(Test_Cast, testIntCastFromArray);
PHP_METHOD(Test_Cast, testIntCastFromStdClass);
PHP_METHOD(Test_Cast, testIntCastFromVariableStdClass);
PHP_METHOD(Test_Cast, testFloatCastFromFloat);
PHP_METHOD(Test_Cast, testFloatCastFromVariableFloat);
PHP_METHOD(Test_Cast, testFloatCastFromBooleanTrue);
PHP_METHOD(Test_Cast, testFloatCastFromBooleanFalse);
PHP_METHOD(Test_Cast, testFloatCastFromVariableBooleanTrue);
PHP_METHOD(Test_Cast, testFloatCastFromVariableBooleanFalse);
PHP_METHOD(Test_Cast, testFloatCastFromVariableNull);
PHP_METHOD(Test_Cast, testFloatCastFromNull);
PHP_METHOD(Test_Cast, testFloatCastFromVariableEmptyArray);
PHP_METHOD(Test_Cast, testFloatCastFromEmptyArray);
PHP_METHOD(Test_Cast, testFloatCastFromVariableArray);
PHP_METHOD(Test_Cast, testFloatCastFromArray);
PHP_METHOD(Test_Cast, testFloatCastFromStdClass);
PHP_METHOD(Test_Cast, testFloatCastFromVariableStdClass);
PHP_METHOD(Test_Cast, testBooleanCastFromIntTrue1);
PHP_METHOD(Test_Cast, testBooleanCastFromIntTrue2);
PHP_METHOD(Test_Cast, testBooleanCastFromIntFalse);
PHP_METHOD(Test_Cast, testBooleanCastFromObject);
PHP_METHOD(Test_Cast, testBooleanCastFromEmptyArray);
PHP_METHOD(Test_Cast, testBooleanCastFromArray);
PHP_METHOD(Test_Cast, testBooleanCastFromNull);
PHP_METHOD(Test_Cast, testObjectCastFromInt);
PHP_METHOD(Test_Cast, testObjectCastFromFloat);
PHP_METHOD(Test_Cast, testObjectCastFromFalse);
PHP_METHOD(Test_Cast, testObjectCastFromTrue);
PHP_METHOD(Test_Cast, testObjectCastFromNull);
PHP_METHOD(Test_Cast, testObjectCastFromEmptyArray);
PHP_METHOD(Test_Cast, testObjectCastFromArray);
PHP_METHOD(Test_Cast, testObjectCastFromEmptyString);
PHP_METHOD(Test_Cast, testObjectCastFromString);
PHP_METHOD(Test_Cast, testCastStdinToInteger);
PHP_METHOD(Test_Cast, testCastStdoutToInteger);
PHP_METHOD(Test_Cast, testCastFileResourceToInteger);
PHP_METHOD(Test_Cast, testArrayCastFromVariableArray);
PHP_METHOD(Test_Cast, testArrayCastFromVariableTrue);
PHP_METHOD(Test_Cast, testArrayCastFromVariableFalse);
PHP_METHOD(Test_Cast, testArrayCastFromVariableNull);
PHP_METHOD(Test_Cast, testArrayCastFromVariableInteger);
PHP_METHOD(Test_Cast, testArrayCastFromVariableFloat);
PHP_METHOD(Test_Cast, testArrayCastFromVariableString);
PHP_METHOD(Test_Cast, testArrayCastFromVariableStdClass);

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testintcastfromfloat, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testintcastfromfloat, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testintcastfromvariablefloat, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testintcastfromvariablefloat, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testintcastfrombooleantrue, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testintcastfrombooleantrue, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testintcastfrombooleanfalse, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testintcastfrombooleanfalse, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testintcastfromvariablebooleantrue, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testintcastfromvariablebooleantrue, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testintcastfromvariablebooleanfalse, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testintcastfromvariablebooleanfalse, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testintcastfromvariablenull, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testintcastfromvariablenull, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testintcastfromstringvalue, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testintcastfromstringvalue, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testintcastfromvariablestring, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testintcastfromvariablestring, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testintcastfromparameterstring, 0, 1, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testintcastfromparameterstring, 0, 1, IS_LONG, NULL, 0)
#endif
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, a, IS_STRING, 0)
#else
	ZEND_ARG_INFO(0, a)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testintcastfromnull, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testintcastfromnull, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testintcastfromvariableemptyarray, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testintcastfromvariableemptyarray, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testintcastfromemptyarray, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testintcastfromemptyarray, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testintcastfromvariablearray, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testintcastfromvariablearray, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testintcastfromarray, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testintcastfromarray, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testintcastfromstdclass, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testintcastfromstdclass, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testintcastfromvariablestdclass, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testintcastfromvariablestdclass, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testfloatcastfromfloat, 0, 0, IS_DOUBLE, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testfloatcastfromfloat, 0, 0, IS_DOUBLE, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testfloatcastfromvariablefloat, 0, 0, IS_DOUBLE, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testfloatcastfromvariablefloat, 0, 0, IS_DOUBLE, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testfloatcastfrombooleantrue, 0, 0, IS_DOUBLE, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testfloatcastfrombooleantrue, 0, 0, IS_DOUBLE, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testfloatcastfrombooleanfalse, 0, 0, IS_DOUBLE, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testfloatcastfrombooleanfalse, 0, 0, IS_DOUBLE, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testfloatcastfromvariablebooleantrue, 0, 0, IS_DOUBLE, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testfloatcastfromvariablebooleantrue, 0, 0, IS_DOUBLE, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testfloatcastfromvariablebooleanfalse, 0, 0, IS_DOUBLE, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testfloatcastfromvariablebooleanfalse, 0, 0, IS_DOUBLE, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testfloatcastfromvariablenull, 0, 0, IS_DOUBLE, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testfloatcastfromvariablenull, 0, 0, IS_DOUBLE, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testfloatcastfromnull, 0, 0, IS_DOUBLE, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testfloatcastfromnull, 0, 0, IS_DOUBLE, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testfloatcastfromvariableemptyarray, 0, 0, IS_DOUBLE, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testfloatcastfromvariableemptyarray, 0, 0, IS_DOUBLE, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testfloatcastfromemptyarray, 0, 0, IS_DOUBLE, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testfloatcastfromemptyarray, 0, 0, IS_DOUBLE, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testfloatcastfromvariablearray, 0, 0, IS_DOUBLE, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testfloatcastfromvariablearray, 0, 0, IS_DOUBLE, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testfloatcastfromarray, 0, 0, IS_DOUBLE, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testfloatcastfromarray, 0, 0, IS_DOUBLE, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testfloatcastfromstdclass, 0, 0, IS_DOUBLE, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testfloatcastfromstdclass, 0, 0, IS_DOUBLE, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testfloatcastfromvariablestdclass, 0, 0, IS_DOUBLE, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testfloatcastfromvariablestdclass, 0, 0, IS_DOUBLE, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testbooleancastfrominttrue1, 0, 0, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testbooleancastfrominttrue1, 0, 0, _IS_BOOL, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testbooleancastfrominttrue2, 0, 0, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testbooleancastfrominttrue2, 0, 0, _IS_BOOL, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testbooleancastfromintfalse, 0, 0, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testbooleancastfromintfalse, 0, 0, _IS_BOOL, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testbooleancastfromobject, 0, 0, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testbooleancastfromobject, 0, 0, _IS_BOOL, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testbooleancastfromemptyarray, 0, 0, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testbooleancastfromemptyarray, 0, 0, _IS_BOOL, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testbooleancastfromarray, 0, 0, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testbooleancastfromarray, 0, 0, _IS_BOOL, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testbooleancastfromnull, 0, 0, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testbooleancastfromnull, 0, 0, _IS_BOOL, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testcaststdintointeger, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testcaststdintointeger, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testcaststdouttointeger, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testcaststdouttointeger, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testcastfileresourcetointeger, 0, 1, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_cast_testcastfileresourcetointeger, 0, 1, IS_LONG, NULL, 0)
#endif
	ZEND_ARG_INFO(0, fileName)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_cast_method_entry) {
	PHP_ME(Test_Cast, testIntCastFromFloat, arginfo_test_cast_testintcastfromfloat, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testIntCastFromVariableFloat, arginfo_test_cast_testintcastfromvariablefloat, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testIntCastFromBooleanTrue, arginfo_test_cast_testintcastfrombooleantrue, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testIntCastFromBooleanFalse, arginfo_test_cast_testintcastfrombooleanfalse, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testIntCastFromVariableBooleanTrue, arginfo_test_cast_testintcastfromvariablebooleantrue, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testIntCastFromVariableBooleanFalse, arginfo_test_cast_testintcastfromvariablebooleanfalse, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testIntCastFromVariableNull, arginfo_test_cast_testintcastfromvariablenull, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testIntCastFromStringValue, arginfo_test_cast_testintcastfromstringvalue, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testIntCastFromVariableString, arginfo_test_cast_testintcastfromvariablestring, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testIntCastFromParameterString, arginfo_test_cast_testintcastfromparameterstring, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testIntCastFromNull, arginfo_test_cast_testintcastfromnull, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testIntCastFromVariableEmptyArray, arginfo_test_cast_testintcastfromvariableemptyarray, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testIntCastFromEmptyArray, arginfo_test_cast_testintcastfromemptyarray, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testIntCastFromVariableArray, arginfo_test_cast_testintcastfromvariablearray, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testIntCastFromArray, arginfo_test_cast_testintcastfromarray, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testIntCastFromStdClass, arginfo_test_cast_testintcastfromstdclass, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testIntCastFromVariableStdClass, arginfo_test_cast_testintcastfromvariablestdclass, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testFloatCastFromFloat, arginfo_test_cast_testfloatcastfromfloat, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testFloatCastFromVariableFloat, arginfo_test_cast_testfloatcastfromvariablefloat, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testFloatCastFromBooleanTrue, arginfo_test_cast_testfloatcastfrombooleantrue, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testFloatCastFromBooleanFalse, arginfo_test_cast_testfloatcastfrombooleanfalse, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testFloatCastFromVariableBooleanTrue, arginfo_test_cast_testfloatcastfromvariablebooleantrue, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testFloatCastFromVariableBooleanFalse, arginfo_test_cast_testfloatcastfromvariablebooleanfalse, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testFloatCastFromVariableNull, arginfo_test_cast_testfloatcastfromvariablenull, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testFloatCastFromNull, arginfo_test_cast_testfloatcastfromnull, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testFloatCastFromVariableEmptyArray, arginfo_test_cast_testfloatcastfromvariableemptyarray, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testFloatCastFromEmptyArray, arginfo_test_cast_testfloatcastfromemptyarray, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testFloatCastFromVariableArray, arginfo_test_cast_testfloatcastfromvariablearray, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testFloatCastFromArray, arginfo_test_cast_testfloatcastfromarray, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testFloatCastFromStdClass, arginfo_test_cast_testfloatcastfromstdclass, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testFloatCastFromVariableStdClass, arginfo_test_cast_testfloatcastfromvariablestdclass, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testBooleanCastFromIntTrue1, arginfo_test_cast_testbooleancastfrominttrue1, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testBooleanCastFromIntTrue2, arginfo_test_cast_testbooleancastfrominttrue2, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testBooleanCastFromIntFalse, arginfo_test_cast_testbooleancastfromintfalse, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testBooleanCastFromObject, arginfo_test_cast_testbooleancastfromobject, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testBooleanCastFromEmptyArray, arginfo_test_cast_testbooleancastfromemptyarray, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testBooleanCastFromArray, arginfo_test_cast_testbooleancastfromarray, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testBooleanCastFromNull, arginfo_test_cast_testbooleancastfromnull, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testObjectCastFromInt, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testObjectCastFromFloat, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testObjectCastFromFalse, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testObjectCastFromTrue, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testObjectCastFromNull, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testObjectCastFromEmptyArray, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testObjectCastFromArray, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testObjectCastFromEmptyString, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testObjectCastFromString, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testCastStdinToInteger, arginfo_test_cast_testcaststdintointeger, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testCastStdoutToInteger, arginfo_test_cast_testcaststdouttointeger, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testCastFileResourceToInteger, arginfo_test_cast_testcastfileresourcetointeger, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testArrayCastFromVariableArray, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testArrayCastFromVariableTrue, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testArrayCastFromVariableFalse, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testArrayCastFromVariableNull, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testArrayCastFromVariableInteger, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testArrayCastFromVariableFloat, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testArrayCastFromVariableString, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testArrayCastFromVariableStdClass, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
