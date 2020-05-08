
extern zend_class_entry *stub_operator_ce;

ZEPHIR_INIT_CLASS(Stub_Operator);

PHP_METHOD(Stub_Operator, testIdentical);
PHP_METHOD(Stub_Operator, testIdenticalIfComplex);
PHP_METHOD(Stub_Operator, testIdenticalVarFalse);
PHP_METHOD(Stub_Operator, testIdenticalFalseVar);
PHP_METHOD(Stub_Operator, testBoolComparison);
PHP_METHOD(Stub_Operator, testNewInstanceOperator);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_operator_testidentical, 0, 0, 2)
	ZEND_ARG_INFO(0, param1)
	ZEND_ARG_INFO(0, param2)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_operator_testidenticalifcomplex, 0, 0, 1)
	ZEND_ARG_INFO(0, input)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_operator_testidenticalvarfalse, 0, 0, 1)
	ZEND_ARG_INFO(0, param)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_operator_testidenticalfalsevar, 0, 0, 1)
	ZEND_ARG_INFO(0, param)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_operator_testboolcomparison, 0, 0, 2)
	ZEND_ARG_INFO(0, varTrue)
	ZEND_ARG_INFO(0, varFalse)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_operator_testnewinstanceoperator, 0, 0, 1)
	ZEND_ARG_INFO(0, className)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_operator_method_entry) {
	PHP_ME(Stub_Operator, testIdentical, arginfo_stub_operator_testidentical, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Operator, testIdenticalIfComplex, arginfo_stub_operator_testidenticalifcomplex, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Operator, testIdenticalVarFalse, arginfo_stub_operator_testidenticalvarfalse, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Operator, testIdenticalFalseVar, arginfo_stub_operator_testidenticalfalsevar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Operator, testBoolComparison, arginfo_stub_operator_testboolcomparison, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Operator, testNewInstanceOperator, arginfo_stub_operator_testnewinstanceoperator, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
