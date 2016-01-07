
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

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_cast_testintcastfromparameterstring, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_cast_method_entry) {
	PHP_ME(Test_Cast, testIntCastFromFloat, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testIntCastFromVariableFloat, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testIntCastFromBooleanTrue, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testIntCastFromBooleanFalse, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testIntCastFromVariableBooleanTrue, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testIntCastFromVariableBooleanFalse, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testIntCastFromVariableNull, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testIntCastFromStringValue, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testIntCastFromVariableString, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testIntCastFromParameterString, arginfo_test_cast_testintcastfromparameterstring, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testIntCastFromNull, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testIntCastFromVariableEmptyArray, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testIntCastFromEmptyArray, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testIntCastFromVariableArray, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testIntCastFromArray, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testIntCastFromStdClass, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testIntCastFromVariableStdClass, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testFloatCastFromFloat, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testFloatCastFromVariableFloat, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testFloatCastFromBooleanTrue, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testFloatCastFromBooleanFalse, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testFloatCastFromVariableBooleanTrue, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testFloatCastFromVariableBooleanFalse, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testFloatCastFromVariableNull, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testFloatCastFromNull, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testFloatCastFromVariableEmptyArray, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testFloatCastFromEmptyArray, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testFloatCastFromVariableArray, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testFloatCastFromArray, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testFloatCastFromStdClass, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testFloatCastFromVariableStdClass, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testBooleanCastFromIntTrue1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testBooleanCastFromIntTrue2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testBooleanCastFromIntFalse, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testBooleanCastFromObject, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testBooleanCastFromEmptyArray, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testBooleanCastFromArray, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testBooleanCastFromNull, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testObjectCastFromInt, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testObjectCastFromFloat, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testObjectCastFromFalse, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testObjectCastFromTrue, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testObjectCastFromNull, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testObjectCastFromEmptyArray, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testObjectCastFromArray, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testObjectCastFromEmptyString, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testObjectCastFromString, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
