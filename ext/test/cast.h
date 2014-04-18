
extern zend_class_entry *test_cast_ce;

ZEPHIR_INIT_CLASS(Test_Cast);

PHP_METHOD(Test_Cast, testIntCastFromFloat);
PHP_METHOD(Test_Cast, testIntCastFromVariableFloat);
PHP_METHOD(Test_Cast, testIntCastFromBooleanTrue);
PHP_METHOD(Test_Cast, testIntCastFromBooleanFalse);
PHP_METHOD(Test_Cast, testIntCastFromVariableBooleanTrue);
PHP_METHOD(Test_Cast, testIntCastFromVariableBooleanFalse);
PHP_METHOD(Test_Cast, testIntCastFromVariableNull);
PHP_METHOD(Test_Cast, testIntCastFromNull);
PHP_METHOD(Test_Cast, testIntCastFromVariableEmptyArray);
PHP_METHOD(Test_Cast, testIntCastFromEmptyArray);
PHP_METHOD(Test_Cast, testIntCastFromVariableArray);
PHP_METHOD(Test_Cast, testIntCastFromArray);
PHP_METHOD(Test_Cast, testIntCastFromStdClass);
PHP_METHOD(Test_Cast, testIntCastFromVariableStdClass);
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

ZEPHIR_INIT_FUNCS(test_cast_method_entry) {
	PHP_ME(Test_Cast, testIntCastFromFloat, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testIntCastFromVariableFloat, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testIntCastFromBooleanTrue, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testIntCastFromBooleanFalse, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testIntCastFromVariableBooleanTrue, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testIntCastFromVariableBooleanFalse, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testIntCastFromVariableNull, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testIntCastFromNull, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testIntCastFromVariableEmptyArray, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testIntCastFromEmptyArray, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testIntCastFromVariableArray, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testIntCastFromArray, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testIntCastFromStdClass, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testIntCastFromVariableStdClass, NULL, ZEND_ACC_PUBLIC)
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
  PHP_FE_END
};
