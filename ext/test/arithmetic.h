
extern zend_class_entry *test_arithmetic_ce;

ZEPHIR_INIT_CLASS(Test_Arithmetic);

PHP_METHOD(Test_Arithmetic, intSum);
PHP_METHOD(Test_Arithmetic, int2Sum);
PHP_METHOD(Test_Arithmetic, intSumSimple);
PHP_METHOD(Test_Arithmetic, boolSum);
PHP_METHOD(Test_Arithmetic, bool2Sum);
PHP_METHOD(Test_Arithmetic, bool3Sum);
PHP_METHOD(Test_Arithmetic, boolSumSimple);
PHP_METHOD(Test_Arithmetic, doubleSum);
PHP_METHOD(Test_Arithmetic, double2Sum);
PHP_METHOD(Test_Arithmetic, doubleSumSimple);
PHP_METHOD(Test_Arithmetic, doubleSum2Simple);
PHP_METHOD(Test_Arithmetic, varSum);
PHP_METHOD(Test_Arithmetic, varSumSimple);
PHP_METHOD(Test_Arithmetic, intDoubleSum);
PHP_METHOD(Test_Arithmetic, intDoubleSumSimple);
PHP_METHOD(Test_Arithmetic, doubleIntSum);
PHP_METHOD(Test_Arithmetic, doubleIntSumSimple);
PHP_METHOD(Test_Arithmetic, varIntSum);
PHP_METHOD(Test_Arithmetic, intVarSum);
PHP_METHOD(Test_Arithmetic, intVarImplicitCastSum);
PHP_METHOD(Test_Arithmetic, intVarImplicitCast2Sum);
PHP_METHOD(Test_Arithmetic, complexSum);
PHP_METHOD(Test_Arithmetic, complex2Sum);
PHP_METHOD(Test_Arithmetic, complex3Sum);
PHP_METHOD(Test_Arithmetic, complex4Sum);
PHP_METHOD(Test_Arithmetic, complex5Sum);
PHP_METHOD(Test_Arithmetic, complex6Sum);
PHP_METHOD(Test_Arithmetic, complex7Sum);
PHP_METHOD(Test_Arithmetic, complex9Sum);
PHP_METHOD(Test_Arithmetic, complex10Sum);
PHP_METHOD(Test_Arithmetic, complex11Sum);
PHP_METHOD(Test_Arithmetic, complex12Sum);
PHP_METHOD(Test_Arithmetic, complex13Sum);
PHP_METHOD(Test_Arithmetic, complex14Sum);
PHP_METHOD(Test_Arithmetic, complex15Sum);
PHP_METHOD(Test_Arithmetic, complex16Sum);
PHP_METHOD(Test_Arithmetic, complex17Sum);
PHP_METHOD(Test_Arithmetic, complex18Sum);
PHP_METHOD(Test_Arithmetic, complex19Sum);
PHP_METHOD(Test_Arithmetic, complex20Sum);
PHP_METHOD(Test_Arithmetic, complex21Sum);
PHP_METHOD(Test_Arithmetic, complex22Sum);
PHP_METHOD(Test_Arithmetic, complex23Sum);
PHP_METHOD(Test_Arithmetic, complex24Sum);
PHP_METHOD(Test_Arithmetic, addSum1);
PHP_METHOD(Test_Arithmetic, addSum2);
PHP_METHOD(Test_Arithmetic, addSum3);
PHP_METHOD(Test_Arithmetic, addSum4);
PHP_METHOD(Test_Arithmetic, addSum5);
PHP_METHOD(Test_Arithmetic, addSum6);
PHP_METHOD(Test_Arithmetic, addSum7);
PHP_METHOD(Test_Arithmetic, addSum8);
PHP_METHOD(Test_Arithmetic, addSum9);
PHP_METHOD(Test_Arithmetic, addSum10);
PHP_METHOD(Test_Arithmetic, addSum11);
PHP_METHOD(Test_Arithmetic, addSum12);
PHP_METHOD(Test_Arithmetic, addSum13);
PHP_METHOD(Test_Arithmetic, addSum14);
PHP_METHOD(Test_Arithmetic, addSum15);
PHP_METHOD(Test_Arithmetic, addSum16);
PHP_METHOD(Test_Arithmetic, addSum17);
PHP_METHOD(Test_Arithmetic, addSum18);
PHP_METHOD(Test_Arithmetic, addSum19);
PHP_METHOD(Test_Arithmetic, addSum20);
PHP_METHOD(Test_Arithmetic, addSum21);
PHP_METHOD(Test_Arithmetic, addSum22);
PHP_METHOD(Test_Arithmetic, addSum23);
PHP_METHOD(Test_Arithmetic, complexSub);

ZEPHIR_INIT_FUNCS(test_arithmetic_method_entry) {
	PHP_ME(Test_Arithmetic, intSum, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, int2Sum, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, intSumSimple, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, boolSum, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, bool2Sum, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, bool3Sum, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, boolSumSimple, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, doubleSum, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, double2Sum, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, doubleSumSimple, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, doubleSum2Simple, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, varSum, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, varSumSimple, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, intDoubleSum, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, intDoubleSumSimple, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, doubleIntSum, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, doubleIntSumSimple, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, varIntSum, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, intVarSum, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, intVarImplicitCastSum, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, intVarImplicitCast2Sum, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, complexSum, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, complex2Sum, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, complex3Sum, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, complex4Sum, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, complex5Sum, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, complex6Sum, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, complex7Sum, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, complex9Sum, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, complex10Sum, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, complex11Sum, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, complex12Sum, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, complex13Sum, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, complex14Sum, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, complex15Sum, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, complex16Sum, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, complex17Sum, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, complex18Sum, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, complex19Sum, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, complex20Sum, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, complex21Sum, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, complex22Sum, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, complex23Sum, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, complex24Sum, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, addSum1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, addSum2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, addSum3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, addSum4, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, addSum5, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, addSum6, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, addSum7, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, addSum8, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, addSum9, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, addSum10, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, addSum11, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, addSum12, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, addSum13, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, addSum14, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, addSum15, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, addSum16, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, addSum17, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, addSum18, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, addSum19, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, addSum20, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, addSum21, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, addSum22, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, addSum23, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Arithmetic, complexSub, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
