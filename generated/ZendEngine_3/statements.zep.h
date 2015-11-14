
extern zend_class_entry *test_statements_ce;

ZEPHIR_INIT_CLASS(Test_Statements);

PHP_METHOD(Test_Statements, testPropertyAcccessAvoidTmpReuse);
PHP_METHOD(Test_Statements, testElseIf);
PHP_METHOD(Test_Statements, testElseIf1);
PHP_METHOD(Test_Statements, testElseIf2);
PHP_METHOD(Test_Statements, test544Issue);
PHP_METHOD(Test_Statements, test544IssueWithVariable);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_statements_testelseif, 0, 0, 1)
	ZEND_ARG_INFO(0, num)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_statements_testelseif1, 0, 0, 1)
	ZEND_ARG_INFO(0, num)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_statements_testelseif2, 0, 0, 2)
	ZEND_ARG_INFO(0, num)
	ZEND_ARG_INFO(0, total)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_statements_test544issue, 0, 0, 1)
	ZEND_ARG_INFO(0, step)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_statements_test544issuewithvariable, 0, 0, 1)
	ZEND_ARG_INFO(0, step)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_statements_method_entry) {
	PHP_ME(Test_Statements, testPropertyAcccessAvoidTmpReuse, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Statements, testElseIf, arginfo_test_statements_testelseif, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Statements, testElseIf1, arginfo_test_statements_testelseif1, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Statements, testElseIf2, arginfo_test_statements_testelseif2, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Statements, test544Issue, arginfo_test_statements_test544issue, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Statements, test544IssueWithVariable, arginfo_test_statements_test544issuewithvariable, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
