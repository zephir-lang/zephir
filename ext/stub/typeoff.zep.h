
extern zend_class_entry *stub_typeoff_ce;

ZEPHIR_INIT_CLASS(Stub_Typeoff);

PHP_METHOD(Stub_Typeoff, testNativeStringFalse);
PHP_METHOD(Stub_Typeoff, testNativeStringTrue);
PHP_METHOD(Stub_Typeoff, testNativeIntFalse);
PHP_METHOD(Stub_Typeoff, testNativeIntTrue);
PHP_METHOD(Stub_Typeoff, testNativeDoubleTrue);
PHP_METHOD(Stub_Typeoff, testNativeBoolTrue);
PHP_METHOD(Stub_Typeoff, testNotBoolTrue);
PHP_METHOD(Stub_Typeoff, testNativeBoolFalse);
PHP_METHOD(Stub_Typeoff, testArrayFalse);
PHP_METHOD(Stub_Typeoff, testArrayTrue);
PHP_METHOD(Stub_Typeoff, testClassPropertyAccess);
PHP_METHOD(Stub_Typeoff, testUnknownTypeOf);
PHP_METHOD(Stub_Typeoff, testCallableTypeOf);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_typeoff_testunknowntypeof, 0, 0, 1)
	ZEND_ARG_INFO(0, u)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_typeoff_testcallabletypeof, 0, 0, 1)
	ZEND_ARG_INFO(0, cb)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_typeoff_method_entry) {
	PHP_ME(Stub_Typeoff, testNativeStringFalse, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Typeoff, testNativeStringTrue, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Typeoff, testNativeIntFalse, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Typeoff, testNativeIntTrue, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Typeoff, testNativeDoubleTrue, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Typeoff, testNativeBoolTrue, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Typeoff, testNotBoolTrue, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Typeoff, testNativeBoolFalse, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Typeoff, testArrayFalse, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Typeoff, testArrayTrue, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Typeoff, testClassPropertyAccess, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Typeoff, testUnknownTypeOf, arginfo_stub_typeoff_testunknowntypeof, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Typeoff, testCallableTypeOf, arginfo_stub_typeoff_testcallabletypeof, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
