
extern zend_class_entry *test_bitwise_ce;

ZEPHIR_INIT_CLASS(Test_Bitwise);

PHP_METHOD(Test_Bitwise, intAnd);
PHP_METHOD(Test_Bitwise, int2And);
PHP_METHOD(Test_Bitwise, intAndSimple);
PHP_METHOD(Test_Bitwise, boolAnd);
PHP_METHOD(Test_Bitwise, bool2And);
PHP_METHOD(Test_Bitwise, bool3And);
PHP_METHOD(Test_Bitwise, boolAndSimple);
PHP_METHOD(Test_Bitwise, doubleAnd);
PHP_METHOD(Test_Bitwise, double2And);
PHP_METHOD(Test_Bitwise, doubleAndSimple);
PHP_METHOD(Test_Bitwise, doubleAnd2Simple);
PHP_METHOD(Test_Bitwise, varAnd);
PHP_METHOD(Test_Bitwise, varAndSimple);
PHP_METHOD(Test_Bitwise, intDoubleAnd);
PHP_METHOD(Test_Bitwise, intDoubleAndSimple);
PHP_METHOD(Test_Bitwise, doubleIntAnd);
PHP_METHOD(Test_Bitwise, doubleIntAndSimple);
PHP_METHOD(Test_Bitwise, varIntAnd);
PHP_METHOD(Test_Bitwise, intVarAnd);
PHP_METHOD(Test_Bitwise, intVarImplicitCastAnd);
PHP_METHOD(Test_Bitwise, intVarImplicitCast2And);
PHP_METHOD(Test_Bitwise, complexAnd);
PHP_METHOD(Test_Bitwise, complex2And);
PHP_METHOD(Test_Bitwise, complex3And);
PHP_METHOD(Test_Bitwise, complex4And);
PHP_METHOD(Test_Bitwise, complex5And);
PHP_METHOD(Test_Bitwise, complex6And);
PHP_METHOD(Test_Bitwise, complex7And);
PHP_METHOD(Test_Bitwise, complex9And);
PHP_METHOD(Test_Bitwise, complex10And);
PHP_METHOD(Test_Bitwise, complex11And);
PHP_METHOD(Test_Bitwise, complex12And);
PHP_METHOD(Test_Bitwise, complex13And);
PHP_METHOD(Test_Bitwise, complex14And);
PHP_METHOD(Test_Bitwise, complex15And);
PHP_METHOD(Test_Bitwise, complex16And);
PHP_METHOD(Test_Bitwise, complex17And);
PHP_METHOD(Test_Bitwise, complex18And);
PHP_METHOD(Test_Bitwise, complex19And);
PHP_METHOD(Test_Bitwise, complex20And);
PHP_METHOD(Test_Bitwise, complex21And);
PHP_METHOD(Test_Bitwise, complex22And);
PHP_METHOD(Test_Bitwise, complex23And);
PHP_METHOD(Test_Bitwise, complex24And);
PHP_METHOD(Test_Bitwise, intOr);
PHP_METHOD(Test_Bitwise, int2Or);
PHP_METHOD(Test_Bitwise, intOrSimple);
PHP_METHOD(Test_Bitwise, boolOr);
PHP_METHOD(Test_Bitwise, bool2Or);
PHP_METHOD(Test_Bitwise, bool3Or);
PHP_METHOD(Test_Bitwise, bool4Or);
PHP_METHOD(Test_Bitwise, boolOrSimple);
PHP_METHOD(Test_Bitwise, doubleOr);
PHP_METHOD(Test_Bitwise, double2Or);
PHP_METHOD(Test_Bitwise, doubleOrSimple);
PHP_METHOD(Test_Bitwise, doubleOr2Simple);
PHP_METHOD(Test_Bitwise, varOr);
PHP_METHOD(Test_Bitwise, varOrSimple);
PHP_METHOD(Test_Bitwise, intDoubleOr);
PHP_METHOD(Test_Bitwise, intDoubleOrSimple);
PHP_METHOD(Test_Bitwise, doubleIntOr);
PHP_METHOD(Test_Bitwise, doubleIntOrSimple);
PHP_METHOD(Test_Bitwise, varIntOr);
PHP_METHOD(Test_Bitwise, intVarOr);
PHP_METHOD(Test_Bitwise, intVarImplicitCastOr);
PHP_METHOD(Test_Bitwise, intVarImplicitCast2Or);
PHP_METHOD(Test_Bitwise, complexOr);
PHP_METHOD(Test_Bitwise, complex2Or);
PHP_METHOD(Test_Bitwise, complex3Or);
PHP_METHOD(Test_Bitwise, complex4Or);
PHP_METHOD(Test_Bitwise, complex5Or);
PHP_METHOD(Test_Bitwise, complex6Or);
PHP_METHOD(Test_Bitwise, complex7Or);
PHP_METHOD(Test_Bitwise, complex9Or);
PHP_METHOD(Test_Bitwise, complex10Or);
PHP_METHOD(Test_Bitwise, complex11Or);
PHP_METHOD(Test_Bitwise, complex12Or);
PHP_METHOD(Test_Bitwise, complex13Or);
PHP_METHOD(Test_Bitwise, complex14Or);
PHP_METHOD(Test_Bitwise, complex15Or);
PHP_METHOD(Test_Bitwise, complex16Or);
PHP_METHOD(Test_Bitwise, complex17Or);
PHP_METHOD(Test_Bitwise, complex18Or);
PHP_METHOD(Test_Bitwise, complex19Or);
PHP_METHOD(Test_Bitwise, complex20Or);
PHP_METHOD(Test_Bitwise, complex21Or);
PHP_METHOD(Test_Bitwise, complex22Or);
PHP_METHOD(Test_Bitwise, complex23Or);
PHP_METHOD(Test_Bitwise, complex24Or);
PHP_METHOD(Test_Bitwise, intShiftLeft);
PHP_METHOD(Test_Bitwise, int2ShiftLeft);
PHP_METHOD(Test_Bitwise, intShiftLeftSimple);
PHP_METHOD(Test_Bitwise, intShiftRight);
PHP_METHOD(Test_Bitwise, int2ShiftRight);
PHP_METHOD(Test_Bitwise, intShiftRightSimple);

ZEPHIR_INIT_FUNCS(test_bitwise_method_entry) {
	PHP_ME(Test_Bitwise, intAnd, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, int2And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, intAndSimple, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, boolAnd, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, bool2And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, bool3And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, boolAndSimple, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, doubleAnd, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, double2And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, doubleAndSimple, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, doubleAnd2Simple, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, varAnd, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, varAndSimple, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, intDoubleAnd, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, intDoubleAndSimple, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, doubleIntAnd, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, doubleIntAndSimple, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, varIntAnd, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, intVarAnd, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, intVarImplicitCastAnd, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, intVarImplicitCast2And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, complexAnd, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, complex2And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, complex3And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, complex4And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, complex5And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, complex6And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, complex7And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, complex9And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, complex10And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, complex11And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, complex12And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, complex13And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, complex14And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, complex15And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, complex16And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, complex17And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, complex18And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, complex19And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, complex20And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, complex21And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, complex22And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, complex23And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, complex24And, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, intOr, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, int2Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, intOrSimple, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, boolOr, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, bool2Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, bool3Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, bool4Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, boolOrSimple, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, doubleOr, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, double2Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, doubleOrSimple, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, doubleOr2Simple, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, varOr, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, varOrSimple, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, intDoubleOr, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, intDoubleOrSimple, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, doubleIntOr, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, doubleIntOrSimple, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, varIntOr, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, intVarOr, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, intVarImplicitCastOr, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, intVarImplicitCast2Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, complexOr, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, complex2Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, complex3Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, complex4Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, complex5Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, complex6Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, complex7Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, complex9Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, complex10Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, complex11Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, complex12Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, complex13Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, complex14Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, complex15Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, complex16Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, complex17Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, complex18Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, complex19Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, complex20Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, complex21Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, complex22Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, complex23Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, complex24Or, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, intShiftLeft, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, int2ShiftLeft, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, intShiftLeftSimple, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, intShiftRight, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, int2ShiftRight, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Bitwise, intShiftRightSimple, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
