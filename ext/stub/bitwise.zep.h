
extern zend_class_entry *stub_bitwise_ce;

ZEPHIR_INIT_CLASS(Stub_Bitwise);

PHP_METHOD(Stub_Bitwise, intAnd);
PHP_METHOD(Stub_Bitwise, int2And);
PHP_METHOD(Stub_Bitwise, intAndSimple);
PHP_METHOD(Stub_Bitwise, boolAnd);
PHP_METHOD(Stub_Bitwise, bool2And);
PHP_METHOD(Stub_Bitwise, bool3And);
PHP_METHOD(Stub_Bitwise, boolAndSimple);
PHP_METHOD(Stub_Bitwise, doubleAnd);
PHP_METHOD(Stub_Bitwise, double2And);
PHP_METHOD(Stub_Bitwise, doubleAndSimple);
PHP_METHOD(Stub_Bitwise, doubleAnd2Simple);
PHP_METHOD(Stub_Bitwise, varAnd);
PHP_METHOD(Stub_Bitwise, varAndSimple);
PHP_METHOD(Stub_Bitwise, intDoubleAnd);
PHP_METHOD(Stub_Bitwise, intDoubleAndSimple);
PHP_METHOD(Stub_Bitwise, doubleIntAnd);
PHP_METHOD(Stub_Bitwise, doubleIntAndSimple);
PHP_METHOD(Stub_Bitwise, varIntAnd);
PHP_METHOD(Stub_Bitwise, intVarAnd);
PHP_METHOD(Stub_Bitwise, intVarImplicitCastAnd);
PHP_METHOD(Stub_Bitwise, intVarImplicitCast2And);
PHP_METHOD(Stub_Bitwise, complexAnd);
PHP_METHOD(Stub_Bitwise, complex2And);
PHP_METHOD(Stub_Bitwise, complex3And);
PHP_METHOD(Stub_Bitwise, complex4And);
PHP_METHOD(Stub_Bitwise, complex5And);
PHP_METHOD(Stub_Bitwise, complex6And);
PHP_METHOD(Stub_Bitwise, complex7And);
PHP_METHOD(Stub_Bitwise, complex9And);
PHP_METHOD(Stub_Bitwise, complex10And);
PHP_METHOD(Stub_Bitwise, complex11And);
PHP_METHOD(Stub_Bitwise, complex12And);
PHP_METHOD(Stub_Bitwise, complex13And);
PHP_METHOD(Stub_Bitwise, complex14And);
PHP_METHOD(Stub_Bitwise, complex15And);
PHP_METHOD(Stub_Bitwise, complex16And);
PHP_METHOD(Stub_Bitwise, complex17And);
PHP_METHOD(Stub_Bitwise, complex18And);
PHP_METHOD(Stub_Bitwise, complex19And);
PHP_METHOD(Stub_Bitwise, complex20And);
PHP_METHOD(Stub_Bitwise, complex21And);
PHP_METHOD(Stub_Bitwise, complex22And);
PHP_METHOD(Stub_Bitwise, complex23And);
PHP_METHOD(Stub_Bitwise, complex24And);
PHP_METHOD(Stub_Bitwise, intOr);
PHP_METHOD(Stub_Bitwise, int2Or);
PHP_METHOD(Stub_Bitwise, intOrSimple);
PHP_METHOD(Stub_Bitwise, boolOr);
PHP_METHOD(Stub_Bitwise, bool2Or);
PHP_METHOD(Stub_Bitwise, bool3Or);
PHP_METHOD(Stub_Bitwise, bool4Or);
PHP_METHOD(Stub_Bitwise, boolOrSimple);
PHP_METHOD(Stub_Bitwise, doubleOr);
PHP_METHOD(Stub_Bitwise, double2Or);
PHP_METHOD(Stub_Bitwise, doubleOrSimple);
PHP_METHOD(Stub_Bitwise, doubleOr2Simple);
PHP_METHOD(Stub_Bitwise, varOr);
PHP_METHOD(Stub_Bitwise, varOrSimple);
PHP_METHOD(Stub_Bitwise, intDoubleOr);
PHP_METHOD(Stub_Bitwise, intDoubleOrSimple);
PHP_METHOD(Stub_Bitwise, doubleIntOr);
PHP_METHOD(Stub_Bitwise, doubleIntOrSimple);
PHP_METHOD(Stub_Bitwise, varIntOr);
PHP_METHOD(Stub_Bitwise, intVarOr);
PHP_METHOD(Stub_Bitwise, intVarImplicitCastOr);
PHP_METHOD(Stub_Bitwise, intVarImplicitCast2Or);
PHP_METHOD(Stub_Bitwise, complexOr);
PHP_METHOD(Stub_Bitwise, complex2Or);
PHP_METHOD(Stub_Bitwise, complex3Or);
PHP_METHOD(Stub_Bitwise, complex4Or);
PHP_METHOD(Stub_Bitwise, complex5Or);
PHP_METHOD(Stub_Bitwise, complex6Or);
PHP_METHOD(Stub_Bitwise, complex7Or);
PHP_METHOD(Stub_Bitwise, complex9Or);
PHP_METHOD(Stub_Bitwise, complex10Or);
PHP_METHOD(Stub_Bitwise, complex11Or);
PHP_METHOD(Stub_Bitwise, complex12Or);
PHP_METHOD(Stub_Bitwise, complex13Or);
PHP_METHOD(Stub_Bitwise, complex14Or);
PHP_METHOD(Stub_Bitwise, complex15Or);
PHP_METHOD(Stub_Bitwise, complex16Or);
PHP_METHOD(Stub_Bitwise, complex17Or);
PHP_METHOD(Stub_Bitwise, complex18Or);
PHP_METHOD(Stub_Bitwise, complex19Or);
PHP_METHOD(Stub_Bitwise, complex20Or);
PHP_METHOD(Stub_Bitwise, complex21Or);
PHP_METHOD(Stub_Bitwise, complex22Or);
PHP_METHOD(Stub_Bitwise, complex23Or);
PHP_METHOD(Stub_Bitwise, complex24Or);
PHP_METHOD(Stub_Bitwise, intShiftLeft);
PHP_METHOD(Stub_Bitwise, int2ShiftLeft);
PHP_METHOD(Stub_Bitwise, intShiftLeftSimple);
PHP_METHOD(Stub_Bitwise, intShiftRight);
PHP_METHOD(Stub_Bitwise, int2ShiftRight);
PHP_METHOD(Stub_Bitwise, intShiftRightSimple);
PHP_METHOD(Stub_Bitwise, testBitwiseNot);
PHP_METHOD(Stub_Bitwise, testBitwiseAndNot);
PHP_METHOD(Stub_Bitwise, getInt);
PHP_METHOD(Stub_Bitwise, testbitwiseXor);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_bitwise_testbitwisenot, 0, 0, 1)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, a)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_bitwise_testbitwiseandnot, 0, 0, 2)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, a)
#endif
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, b)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_bitwise_getint, 0, 1, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_bitwise_getint, 0, 1, IS_LONG, NULL, 0)
#endif
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, num, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, num)
#endif
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_bitwise_method_entry) {
	PHP_ME(Stub_Bitwise, intAnd, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, int2And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, intAndSimple, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, boolAnd, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, bool2And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, bool3And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, boolAndSimple, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, doubleAnd, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, double2And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, doubleAndSimple, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, doubleAnd2Simple, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, varAnd, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, varAndSimple, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, intDoubleAnd, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, intDoubleAndSimple, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, doubleIntAnd, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, doubleIntAndSimple, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, varIntAnd, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, intVarAnd, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, intVarImplicitCastAnd, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, intVarImplicitCast2And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, complexAnd, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, complex2And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, complex3And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, complex4And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, complex5And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, complex6And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, complex7And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, complex9And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, complex10And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, complex11And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, complex12And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, complex13And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, complex14And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, complex15And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, complex16And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, complex17And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, complex18And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, complex19And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, complex20And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, complex21And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, complex22And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, complex23And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, complex24And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, intOr, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, int2Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, intOrSimple, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, boolOr, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, bool2Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, bool3Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, bool4Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, boolOrSimple, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, doubleOr, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, double2Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, doubleOrSimple, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, doubleOr2Simple, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, varOr, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, varOrSimple, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, intDoubleOr, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, intDoubleOrSimple, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, doubleIntOr, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, doubleIntOrSimple, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, varIntOr, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, intVarOr, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, intVarImplicitCastOr, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, intVarImplicitCast2Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, complexOr, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, complex2Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, complex3Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, complex4Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, complex5Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, complex6Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, complex7Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, complex9Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, complex10Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, complex11Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, complex12Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, complex13Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, complex14Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, complex15Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, complex16Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, complex17Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, complex18Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, complex19Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, complex20Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, complex21Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, complex22Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, complex23Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, complex24Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, intShiftLeft, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, int2ShiftLeft, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, intShiftLeftSimple, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, intShiftRight, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, int2ShiftRight, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, intShiftRightSimple, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, testBitwiseNot, arginfo_stub_bitwise_testbitwisenot, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, testBitwiseAndNot, arginfo_stub_bitwise_testbitwiseandnot, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Bitwise, getInt, arginfo_stub_bitwise_getint, ZEND_ACC_PROTECTED)
	PHP_ME(Stub_Bitwise, testbitwiseXor, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
