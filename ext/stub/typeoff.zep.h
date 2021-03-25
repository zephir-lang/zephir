
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

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_typeoff_testnativestringfalse, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_typeoff_testnativestringtrue, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_typeoff_testnativeintfalse, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_typeoff_testnativeinttrue, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_typeoff_testnativedoubletrue, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_typeoff_testnativebooltrue, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_typeoff_testnotbooltrue, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_typeoff_testnativeboolfalse, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_typeoff_testarrayfalse, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_typeoff_testarraytrue, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_typeoff_testclasspropertyaccess, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_typeoff_testunknowntypeof, 0, 0, 1)
	ZEND_ARG_INFO(0, u)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_typeoff_testcallabletypeof, 0, 0, 1)
	ZEND_ARG_INFO(0, cb)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_typeoff_method_entry) {
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Typeoff, testNativeStringFalse, arginfo_stub_typeoff_testnativestringfalse, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Typeoff, testNativeStringFalse, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Typeoff, testNativeStringTrue, arginfo_stub_typeoff_testnativestringtrue, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Typeoff, testNativeStringTrue, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Typeoff, testNativeIntFalse, arginfo_stub_typeoff_testnativeintfalse, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Typeoff, testNativeIntFalse, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Typeoff, testNativeIntTrue, arginfo_stub_typeoff_testnativeinttrue, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Typeoff, testNativeIntTrue, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Typeoff, testNativeDoubleTrue, arginfo_stub_typeoff_testnativedoubletrue, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Typeoff, testNativeDoubleTrue, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Typeoff, testNativeBoolTrue, arginfo_stub_typeoff_testnativebooltrue, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Typeoff, testNativeBoolTrue, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Typeoff, testNotBoolTrue, arginfo_stub_typeoff_testnotbooltrue, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Typeoff, testNotBoolTrue, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Typeoff, testNativeBoolFalse, arginfo_stub_typeoff_testnativeboolfalse, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Typeoff, testNativeBoolFalse, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Typeoff, testArrayFalse, arginfo_stub_typeoff_testarrayfalse, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Typeoff, testArrayFalse, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Typeoff, testArrayTrue, arginfo_stub_typeoff_testarraytrue, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Typeoff, testArrayTrue, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Typeoff, testClassPropertyAccess, arginfo_stub_typeoff_testclasspropertyaccess, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Typeoff, testClassPropertyAccess, NULL, ZEND_ACC_PUBLIC)
#endif
	PHP_ME(Stub_Typeoff, testUnknownTypeOf, arginfo_stub_typeoff_testunknowntypeof, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Typeoff, testCallableTypeOf, arginfo_stub_typeoff_testcallabletypeof, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
