
extern zend_class_entry *test_ternary_ce;

ZEPHIR_INIT_CLASS(Test_Ternary);

PHP_METHOD(Test_Ternary, testTernary1);
PHP_METHOD(Test_Ternary, testTernary2);
PHP_METHOD(Test_Ternary, testTernaryComplex1);
PHP_METHOD(Test_Ternary, testTernaryComplex2);
PHP_METHOD(Test_Ternary, testTernaryComplex3);
PHP_METHOD(Test_Ternary, testTernaryWithPromotedTemporaryVariable);
PHP_METHOD(Test_Ternary, testTernaryAfterLetVariable);
PHP_METHOD(Test_Ternary, testShortTernary);
PHP_METHOD(Test_Ternary, testShortTernaryComplex);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_ternary_testternary2, 0, 0, 1)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_ternary_testternarycomplex1, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, y)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_ternary_testternarycomplex2, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, y)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_ternary_testternarycomplex3, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_ternary_testshortternary, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_ternary_testshortternarycomplex, 0, 0, 2)
	ZEND_ARG_INFO(0, left)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_ternary_method_entry) {
	PHP_ME(Test_Ternary, testTernary1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Ternary, testTernary2, arginfo_test_ternary_testternary2, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Ternary, testTernaryComplex1, arginfo_test_ternary_testternarycomplex1, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Ternary, testTernaryComplex2, arginfo_test_ternary_testternarycomplex2, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Ternary, testTernaryComplex3, arginfo_test_ternary_testternarycomplex3, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Ternary, testTernaryWithPromotedTemporaryVariable, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Ternary, testTernaryAfterLetVariable, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Ternary, testShortTernary, arginfo_test_ternary_testshortternary, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Ternary, testShortTernaryComplex, arginfo_test_ternary_testshortternarycomplex, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
