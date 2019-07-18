
extern zend_class_entry *test_operator_ce;

ZEPHIR_INIT_CLASS(Test_Operator);

PHP_METHOD(Test_Operator, testIdentical);
PHP_METHOD(Test_Operator, testIdenticalIfComplex);
PHP_METHOD(Test_Operator, testIdenticalVarFalse);
PHP_METHOD(Test_Operator, testIdenticalFalseVar);
PHP_METHOD(Test_Operator, testBoolComparison);
PHP_METHOD(Test_Operator, testNewInstanceOperator);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_operator_testidentical, 0, 0, 2)
	ZEND_ARG_INFO(0, param1)
	ZEND_ARG_INFO(0, param2)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_operator_testidenticalifcomplex, 0, 0, 1)
	ZEND_ARG_INFO(0, input)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_operator_testidenticalvarfalse, 0, 0, 1)
	ZEND_ARG_INFO(0, param)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_operator_testidenticalfalsevar, 0, 0, 1)
	ZEND_ARG_INFO(0, param)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_operator_testboolcomparison, 0, 0, 2)
	ZEND_ARG_INFO(0, varTrue)
	ZEND_ARG_INFO(0, varFalse)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_operator_testnewinstanceoperator, 0, 0, 1)
	ZEND_ARG_INFO(0, className)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_operator_method_entry) {
	PHP_ME(Test_Operator, testIdentical, arginfo_test_operator_testidentical, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Operator, testIdenticalIfComplex, arginfo_test_operator_testidenticalifcomplex, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Operator, testIdenticalVarFalse, arginfo_test_operator_testidenticalvarfalse, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Operator, testIdenticalFalseVar, arginfo_test_operator_testidenticalfalsevar, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Operator, testBoolComparison, arginfo_test_operator_testboolcomparison, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Operator, testNewInstanceOperator, arginfo_test_operator_testnewinstanceoperator, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
