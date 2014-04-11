
extern zend_class_entry *test_cast_ce;

ZEPHIR_INIT_CLASS(Test_Cast);

PHP_METHOD(Test_Cast, testIntCastFromFloat);
PHP_METHOD(Test_Cast, testIntCastFromBoolean);
PHP_METHOD(Test_Cast, testBooleanCastFromIntTrue1);
PHP_METHOD(Test_Cast, testBooleanCastFromIntTrue2);
PHP_METHOD(Test_Cast, testBooleanCastFromIntFalse);
PHP_METHOD(Test_Cast, testBooleanCastFromObject);
PHP_METHOD(Test_Cast, testBooleanCastFromEmptyArray);
PHP_METHOD(Test_Cast, testBooleanCastFromArray);
PHP_METHOD(Test_Cast, testBooleanCaseFromNull);

ZEPHIR_INIT_FUNCS(test_cast_method_entry) {
	PHP_ME(Test_Cast, testIntCastFromFloat, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testIntCastFromBoolean, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testBooleanCastFromIntTrue1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testBooleanCastFromIntTrue2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testBooleanCastFromIntFalse, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testBooleanCastFromObject, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testBooleanCastFromEmptyArray, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testBooleanCastFromArray, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testBooleanCaseFromNull, NULL, ZEND_ACC_PUBLIC)
  PHP_FE_END
};
