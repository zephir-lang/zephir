
extern zend_class_entry *stub_ternary_ce;

ZEPHIR_INIT_CLASS(Stub_Ternary);

PHP_METHOD(Stub_Ternary, testTernary1);
PHP_METHOD(Stub_Ternary, testTernary2);
PHP_METHOD(Stub_Ternary, testTernaryComplex1);
PHP_METHOD(Stub_Ternary, testTernaryComplex2);
PHP_METHOD(Stub_Ternary, testTernaryComplex3);
PHP_METHOD(Stub_Ternary, testTernaryWithPromotedTemporaryVariable);
PHP_METHOD(Stub_Ternary, testTernaryAfterLetVariable);
PHP_METHOD(Stub_Ternary, testShortTernary);
PHP_METHOD(Stub_Ternary, testShortTernaryComplex);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_ternary_testternary1, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_ternary_testternary2, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, b, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_ternary_testternarycomplex1, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, y)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_ternary_testternarycomplex2, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, y)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_ternary_testternarycomplex3, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_ternary_testternarywithpromotedtemporaryvariable, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_ternary_testternaryafterletvariable, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_ternary_testshortternary, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_ternary_testshortternarycomplex, 0, 0, 2)
	ZEND_ARG_INFO(0, left)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_ternary_method_entry) {
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Ternary, testTernary1, arginfo_stub_ternary_testternary1, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Ternary, testTernary1, NULL, ZEND_ACC_PUBLIC)
#endif
	PHP_ME(Stub_Ternary, testTernary2, arginfo_stub_ternary_testternary2, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Ternary, testTernaryComplex1, arginfo_stub_ternary_testternarycomplex1, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Ternary, testTernaryComplex2, arginfo_stub_ternary_testternarycomplex2, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Ternary, testTernaryComplex3, arginfo_stub_ternary_testternarycomplex3, ZEND_ACC_PUBLIC)
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Ternary, testTernaryWithPromotedTemporaryVariable, arginfo_stub_ternary_testternarywithpromotedtemporaryvariable, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Ternary, testTernaryWithPromotedTemporaryVariable, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Ternary, testTernaryAfterLetVariable, arginfo_stub_ternary_testternaryafterletvariable, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Ternary, testTernaryAfterLetVariable, NULL, ZEND_ACC_PUBLIC)
#endif
	PHP_ME(Stub_Ternary, testShortTernary, arginfo_stub_ternary_testshortternary, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Ternary, testShortTernaryComplex, arginfo_stub_ternary_testshortternarycomplex, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
