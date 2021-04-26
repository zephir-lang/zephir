
extern zend_class_entry *stub_cast_ce;

ZEPHIR_INIT_CLASS(Stub_Cast);

PHP_METHOD(Stub_Cast, testCharCastFromChar);
PHP_METHOD(Stub_Cast, testCharCastFromVariableChar);
PHP_METHOD(Stub_Cast, testStringCastChar);
PHP_METHOD(Stub_Cast, testStringCastVariableChar);
PHP_METHOD(Stub_Cast, testStringCastFromNull);
PHP_METHOD(Stub_Cast, testIntCastFromFloat);
PHP_METHOD(Stub_Cast, testIntCastFromVariableFloat);
PHP_METHOD(Stub_Cast, testIntCastFromChar);
PHP_METHOD(Stub_Cast, testIntCastFromVariableChar);
PHP_METHOD(Stub_Cast, testIntCastFromBooleanTrue);
PHP_METHOD(Stub_Cast, testIntCastFromBooleanFalse);
PHP_METHOD(Stub_Cast, testIntCastFromVariableBooleanTrue);
PHP_METHOD(Stub_Cast, testIntCastFromVariableBooleanFalse);
PHP_METHOD(Stub_Cast, testIntCastFromVariableNull);
PHP_METHOD(Stub_Cast, testIntCastFromStringValue);
PHP_METHOD(Stub_Cast, testIntCastFromVariableString);
PHP_METHOD(Stub_Cast, testIntCastFromParameterString);
PHP_METHOD(Stub_Cast, testIntCastFromNull);
PHP_METHOD(Stub_Cast, testIntCastFromVariableEmptyArray);
PHP_METHOD(Stub_Cast, testIntCastFromEmptyArray);
PHP_METHOD(Stub_Cast, testIntCastFromVariableArray);
PHP_METHOD(Stub_Cast, testIntCastFromArray);
PHP_METHOD(Stub_Cast, testIntCastFromStdClass);
PHP_METHOD(Stub_Cast, testIntCastFromVariableStdClass);
PHP_METHOD(Stub_Cast, testLongCastFromChar);
PHP_METHOD(Stub_Cast, testLongCastFromVariableChar);
PHP_METHOD(Stub_Cast, testFloatCastFromFloat);
PHP_METHOD(Stub_Cast, testFloatCastFromVariableFloat);
PHP_METHOD(Stub_Cast, testFloatCastFromBooleanTrue);
PHP_METHOD(Stub_Cast, testFloatCastFromBooleanFalse);
PHP_METHOD(Stub_Cast, testFloatCastFromVariableBooleanTrue);
PHP_METHOD(Stub_Cast, testFloatCastFromVariableBooleanFalse);
PHP_METHOD(Stub_Cast, testFloatCastFromVariableNull);
PHP_METHOD(Stub_Cast, testFloatCastFromNull);
PHP_METHOD(Stub_Cast, testFloatCastFromVariableEmptyArray);
PHP_METHOD(Stub_Cast, testFloatCastFromEmptyArray);
PHP_METHOD(Stub_Cast, testFloatCastFromVariableArray);
PHP_METHOD(Stub_Cast, testFloatCastFromArray);
PHP_METHOD(Stub_Cast, testFloatCastFromStdClass);
PHP_METHOD(Stub_Cast, testFloatCastFromVariableStdClass);
PHP_METHOD(Stub_Cast, testDoubleCastFromVChar);
PHP_METHOD(Stub_Cast, testDoubleCastFromVariableChar);
PHP_METHOD(Stub_Cast, testBooleanCastFromIntTrue1);
PHP_METHOD(Stub_Cast, testBooleanCastFromIntTrue2);
PHP_METHOD(Stub_Cast, testBooleanCastFromIntFalse);
PHP_METHOD(Stub_Cast, testBooleanCastFromObject);
PHP_METHOD(Stub_Cast, testBooleanCastFromEmptyArray);
PHP_METHOD(Stub_Cast, testBooleanCastFromArray);
PHP_METHOD(Stub_Cast, testBooleanCastFromNull);
PHP_METHOD(Stub_Cast, testBooleanCastFromChar);
PHP_METHOD(Stub_Cast, testBooleanCastFromVariableChar);
PHP_METHOD(Stub_Cast, testObjectCastFromInt);
PHP_METHOD(Stub_Cast, testObjectCastFromFloat);
PHP_METHOD(Stub_Cast, testObjectCastFromFalse);
PHP_METHOD(Stub_Cast, testObjectCastFromTrue);
PHP_METHOD(Stub_Cast, testObjectCastFromNull);
PHP_METHOD(Stub_Cast, testObjectCastFromEmptyArray);
PHP_METHOD(Stub_Cast, testObjectCastFromArray);
PHP_METHOD(Stub_Cast, testObjectCastFromEmptyString);
PHP_METHOD(Stub_Cast, testObjectCastFromString);
PHP_METHOD(Stub_Cast, testCastStdinToInteger);
PHP_METHOD(Stub_Cast, testCastStdoutToInteger);
PHP_METHOD(Stub_Cast, testCastFileResourceToInteger);
PHP_METHOD(Stub_Cast, testArrayCastFromVariableArray);
PHP_METHOD(Stub_Cast, testArrayCastFromVariableTrue);
PHP_METHOD(Stub_Cast, testArrayCastFromVariableFalse);
PHP_METHOD(Stub_Cast, testArrayCastFromVariableNull);
PHP_METHOD(Stub_Cast, testArrayCastFromVariableInteger);
PHP_METHOD(Stub_Cast, testArrayCastFromVariableFloat);
PHP_METHOD(Stub_Cast, testArrayCastFromVariableString);
PHP_METHOD(Stub_Cast, testArrayCastFromVariableStdClass);
PHP_METHOD(Stub_Cast, testIssue828);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testcharcastfromchar, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testcharcastfromvariablechar, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_teststringcastchar, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_teststringcastvariablechar, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_teststringcastfromnull, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testintcastfromfloat, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testintcastfromvariablefloat, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testintcastfromchar, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testintcastfromvariablechar, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testintcastfrombooleantrue, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testintcastfrombooleanfalse, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testintcastfromvariablebooleantrue, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testintcastfromvariablebooleanfalse, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testintcastfromvariablenull, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testintcastfromstringvalue, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testintcastfromvariablestring, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testintcastfromparameterstring, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, a, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testintcastfromnull, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testintcastfromvariableemptyarray, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testintcastfromemptyarray, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testintcastfromvariablearray, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testintcastfromarray, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testintcastfromstdclass, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testintcastfromvariablestdclass, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testlongcastfromchar, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testlongcastfromvariablechar, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testfloatcastfromfloat, 0, 0, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testfloatcastfromvariablefloat, 0, 0, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testfloatcastfrombooleantrue, 0, 0, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testfloatcastfrombooleanfalse, 0, 0, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testfloatcastfromvariablebooleantrue, 0, 0, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testfloatcastfromvariablebooleanfalse, 0, 0, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testfloatcastfromvariablenull, 0, 0, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testfloatcastfromnull, 0, 0, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testfloatcastfromvariableemptyarray, 0, 0, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testfloatcastfromemptyarray, 0, 0, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testfloatcastfromvariablearray, 0, 0, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testfloatcastfromarray, 0, 0, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testfloatcastfromstdclass, 0, 0, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testfloatcastfromvariablestdclass, 0, 0, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testdoublecastfromvchar, 0, 0, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testdoublecastfromvariablechar, 0, 0, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testbooleancastfrominttrue1, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testbooleancastfrominttrue2, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testbooleancastfromintfalse, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testbooleancastfromobject, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testbooleancastfromemptyarray, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testbooleancastfromarray, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testbooleancastfromnull, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testbooleancastfromchar, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testbooleancastfromvariablechar, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_cast_testobjectcastfromint, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_cast_testobjectcastfromfloat, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_cast_testobjectcastfromfalse, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_cast_testobjectcastfromtrue, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_cast_testobjectcastfromnull, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_cast_testobjectcastfromemptyarray, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_cast_testobjectcastfromarray, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_cast_testobjectcastfromemptystring, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_cast_testobjectcastfromstring, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testcaststdintointeger, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testcaststdouttointeger, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testcastfileresourcetointeger, 0, 1, IS_LONG, 0)
	ZEND_ARG_INFO(0, fileName)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testarraycastfromvariablearray, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testarraycastfromvariabletrue, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testarraycastfromvariablefalse, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testarraycastfromvariablenull, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testarraycastfromvariableinteger, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testarraycastfromvariablefloat, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testarraycastfromvariablestring, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testarraycastfromvariablestdclass, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_cast_testissue828, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_cast_method_entry) {
	PHP_ME(Stub_Cast, testCharCastFromChar, arginfo_stub_cast_testcharcastfromchar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testCharCastFromVariableChar, arginfo_stub_cast_testcharcastfromvariablechar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testStringCastChar, arginfo_stub_cast_teststringcastchar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testStringCastVariableChar, arginfo_stub_cast_teststringcastvariablechar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testStringCastFromNull, arginfo_stub_cast_teststringcastfromnull, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testIntCastFromFloat, arginfo_stub_cast_testintcastfromfloat, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testIntCastFromVariableFloat, arginfo_stub_cast_testintcastfromvariablefloat, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testIntCastFromChar, arginfo_stub_cast_testintcastfromchar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testIntCastFromVariableChar, arginfo_stub_cast_testintcastfromvariablechar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testIntCastFromBooleanTrue, arginfo_stub_cast_testintcastfrombooleantrue, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testIntCastFromBooleanFalse, arginfo_stub_cast_testintcastfrombooleanfalse, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testIntCastFromVariableBooleanTrue, arginfo_stub_cast_testintcastfromvariablebooleantrue, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testIntCastFromVariableBooleanFalse, arginfo_stub_cast_testintcastfromvariablebooleanfalse, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testIntCastFromVariableNull, arginfo_stub_cast_testintcastfromvariablenull, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testIntCastFromStringValue, arginfo_stub_cast_testintcastfromstringvalue, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testIntCastFromVariableString, arginfo_stub_cast_testintcastfromvariablestring, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testIntCastFromParameterString, arginfo_stub_cast_testintcastfromparameterstring, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testIntCastFromNull, arginfo_stub_cast_testintcastfromnull, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testIntCastFromVariableEmptyArray, arginfo_stub_cast_testintcastfromvariableemptyarray, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testIntCastFromEmptyArray, arginfo_stub_cast_testintcastfromemptyarray, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testIntCastFromVariableArray, arginfo_stub_cast_testintcastfromvariablearray, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testIntCastFromArray, arginfo_stub_cast_testintcastfromarray, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testIntCastFromStdClass, arginfo_stub_cast_testintcastfromstdclass, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testIntCastFromVariableStdClass, arginfo_stub_cast_testintcastfromvariablestdclass, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testLongCastFromChar, arginfo_stub_cast_testlongcastfromchar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testLongCastFromVariableChar, arginfo_stub_cast_testlongcastfromvariablechar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testFloatCastFromFloat, arginfo_stub_cast_testfloatcastfromfloat, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testFloatCastFromVariableFloat, arginfo_stub_cast_testfloatcastfromvariablefloat, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testFloatCastFromBooleanTrue, arginfo_stub_cast_testfloatcastfrombooleantrue, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testFloatCastFromBooleanFalse, arginfo_stub_cast_testfloatcastfrombooleanfalse, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testFloatCastFromVariableBooleanTrue, arginfo_stub_cast_testfloatcastfromvariablebooleantrue, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testFloatCastFromVariableBooleanFalse, arginfo_stub_cast_testfloatcastfromvariablebooleanfalse, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testFloatCastFromVariableNull, arginfo_stub_cast_testfloatcastfromvariablenull, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testFloatCastFromNull, arginfo_stub_cast_testfloatcastfromnull, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testFloatCastFromVariableEmptyArray, arginfo_stub_cast_testfloatcastfromvariableemptyarray, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testFloatCastFromEmptyArray, arginfo_stub_cast_testfloatcastfromemptyarray, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testFloatCastFromVariableArray, arginfo_stub_cast_testfloatcastfromvariablearray, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testFloatCastFromArray, arginfo_stub_cast_testfloatcastfromarray, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testFloatCastFromStdClass, arginfo_stub_cast_testfloatcastfromstdclass, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testFloatCastFromVariableStdClass, arginfo_stub_cast_testfloatcastfromvariablestdclass, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testDoubleCastFromVChar, arginfo_stub_cast_testdoublecastfromvchar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testDoubleCastFromVariableChar, arginfo_stub_cast_testdoublecastfromvariablechar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testBooleanCastFromIntTrue1, arginfo_stub_cast_testbooleancastfrominttrue1, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testBooleanCastFromIntTrue2, arginfo_stub_cast_testbooleancastfrominttrue2, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testBooleanCastFromIntFalse, arginfo_stub_cast_testbooleancastfromintfalse, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testBooleanCastFromObject, arginfo_stub_cast_testbooleancastfromobject, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testBooleanCastFromEmptyArray, arginfo_stub_cast_testbooleancastfromemptyarray, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testBooleanCastFromArray, arginfo_stub_cast_testbooleancastfromarray, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testBooleanCastFromNull, arginfo_stub_cast_testbooleancastfromnull, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testBooleanCastFromChar, arginfo_stub_cast_testbooleancastfromchar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testBooleanCastFromVariableChar, arginfo_stub_cast_testbooleancastfromvariablechar, ZEND_ACC_PUBLIC)
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Cast, testObjectCastFromInt, arginfo_stub_cast_testobjectcastfromint, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Cast, testObjectCastFromInt, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Cast, testObjectCastFromFloat, arginfo_stub_cast_testobjectcastfromfloat, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Cast, testObjectCastFromFloat, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Cast, testObjectCastFromFalse, arginfo_stub_cast_testobjectcastfromfalse, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Cast, testObjectCastFromFalse, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Cast, testObjectCastFromTrue, arginfo_stub_cast_testobjectcastfromtrue, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Cast, testObjectCastFromTrue, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Cast, testObjectCastFromNull, arginfo_stub_cast_testobjectcastfromnull, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Cast, testObjectCastFromNull, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Cast, testObjectCastFromEmptyArray, arginfo_stub_cast_testobjectcastfromemptyarray, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Cast, testObjectCastFromEmptyArray, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Cast, testObjectCastFromArray, arginfo_stub_cast_testobjectcastfromarray, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Cast, testObjectCastFromArray, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Cast, testObjectCastFromEmptyString, arginfo_stub_cast_testobjectcastfromemptystring, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Cast, testObjectCastFromEmptyString, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Cast, testObjectCastFromString, arginfo_stub_cast_testobjectcastfromstring, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Cast, testObjectCastFromString, NULL, ZEND_ACC_PUBLIC)
#endif
	PHP_ME(Stub_Cast, testCastStdinToInteger, arginfo_stub_cast_testcaststdintointeger, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testCastStdoutToInteger, arginfo_stub_cast_testcaststdouttointeger, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testCastFileResourceToInteger, arginfo_stub_cast_testcastfileresourcetointeger, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testArrayCastFromVariableArray, arginfo_stub_cast_testarraycastfromvariablearray, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testArrayCastFromVariableTrue, arginfo_stub_cast_testarraycastfromvariabletrue, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testArrayCastFromVariableFalse, arginfo_stub_cast_testarraycastfromvariablefalse, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testArrayCastFromVariableNull, arginfo_stub_cast_testarraycastfromvariablenull, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testArrayCastFromVariableInteger, arginfo_stub_cast_testarraycastfromvariableinteger, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testArrayCastFromVariableFloat, arginfo_stub_cast_testarraycastfromvariablefloat, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testArrayCastFromVariableString, arginfo_stub_cast_testarraycastfromvariablestring, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testArrayCastFromVariableStdClass, arginfo_stub_cast_testarraycastfromvariablestdclass, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Cast, testIssue828, arginfo_stub_cast_testissue828, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
