
extern zend_class_entry *stub_compare_ce;

ZEPHIR_INIT_CLASS(Stub_Compare);

PHP_METHOD(Stub_Compare, isLessInt);
PHP_METHOD(Stub_Compare, isGreaterEqual);
PHP_METHOD(Stub_Compare, isLessDouble);
PHP_METHOD(Stub_Compare, isLessThenPi);
PHP_METHOD(Stub_Compare, isMoreThenPi);
PHP_METHOD(Stub_Compare, testVarWithStringEquals);
PHP_METHOD(Stub_Compare, testVarEqualsNull);
PHP_METHOD(Stub_Compare, testNullEqualsVar);
PHP_METHOD(Stub_Compare, testNotIdenticalZeroVar);
PHP_METHOD(Stub_Compare, testNotIdenticalZeroInt);
PHP_METHOD(Stub_Compare, testNotIdenticalZeroLong);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_compare_islessint, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_compare_isgreaterequal, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_compare_islessdouble, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, a, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, b, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_compare_islessthenpi, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, a, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_compare_ismorethenpi, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, a, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_compare_testvarwithstringequals, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_compare_testvarequalsnull, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_compare_testnullequalsvar, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_compare_testnotidenticalzerovar, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_compare_testnotidenticalzeroint, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_compare_testnotidenticalzerolong, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_compare_method_entry) {
	PHP_ME(Stub_Compare, isLessInt, arginfo_stub_compare_islessint, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Compare, isGreaterEqual, arginfo_stub_compare_isgreaterequal, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Compare, isLessDouble, arginfo_stub_compare_islessdouble, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Compare, isLessThenPi, arginfo_stub_compare_islessthenpi, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Compare, isMoreThenPi, arginfo_stub_compare_ismorethenpi, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Compare, testVarWithStringEquals, arginfo_stub_compare_testvarwithstringequals, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Compare, testVarEqualsNull, arginfo_stub_compare_testvarequalsnull, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Compare, testNullEqualsVar, arginfo_stub_compare_testnullequalsvar, ZEND_ACC_PUBLIC)
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Compare, testNotIdenticalZeroVar, arginfo_stub_compare_testnotidenticalzerovar, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Compare, testNotIdenticalZeroVar, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Compare, testNotIdenticalZeroInt, arginfo_stub_compare_testnotidenticalzeroint, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Compare, testNotIdenticalZeroInt, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Compare, testNotIdenticalZeroLong, arginfo_stub_compare_testnotidenticalzerolong, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Compare, testNotIdenticalZeroLong, NULL, ZEND_ACC_PUBLIC)
#endif
	PHP_FE_END
};
