
extern zend_class_entry *test_typeoff_ce;

ZEPHIR_INIT_CLASS(Test_Typeoff);

PHP_METHOD(Test_Typeoff, testNativeStringFalse);
PHP_METHOD(Test_Typeoff, testNativeStringTrue);
PHP_METHOD(Test_Typeoff, testNativeIntFalse);
PHP_METHOD(Test_Typeoff, testNativeIntTrue);
PHP_METHOD(Test_Typeoff, testNativeDoubleTrue);
PHP_METHOD(Test_Typeoff, testNativeBoolTrue);
PHP_METHOD(Test_Typeoff, testNativeBoolFalse);
PHP_METHOD(Test_Typeoff, testArrayFalse);
PHP_METHOD(Test_Typeoff, testArrayTrue);
PHP_METHOD(Test_Typeoff, testClassPropertyAccess);
PHP_METHOD(Test_Typeoff, testUnknownTypeOf);
PHP_METHOD(Test_Typeoff, testCallableTypeOf);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_typeoff_testunknowntypeof, 0, 0, 1)
	ZEND_ARG_INFO(0, u)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_typeoff_testcallabletypeof, 0, 0, 1)
	ZEND_ARG_INFO(0, cb)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_typeoff_method_entry) {
	PHP_ME(Test_Typeoff, testNativeStringFalse, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Typeoff, testNativeStringTrue, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Typeoff, testNativeIntFalse, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Typeoff, testNativeIntTrue, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Typeoff, testNativeDoubleTrue, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Typeoff, testNativeBoolTrue, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Typeoff, testNativeBoolFalse, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Typeoff, testArrayFalse, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Typeoff, testArrayTrue, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Typeoff, testClassPropertyAccess, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Typeoff, testUnknownTypeOf, arginfo_test_typeoff_testunknowntypeof, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Typeoff, testCallableTypeOf, arginfo_test_typeoff_testcallabletypeof, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
