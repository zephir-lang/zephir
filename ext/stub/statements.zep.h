
extern zend_class_entry *stub_statements_ce;

ZEPHIR_INIT_CLASS(Stub_Statements);

PHP_METHOD(Stub_Statements, testPropertyAcccessAvoidTmpReuse);
PHP_METHOD(Stub_Statements, testElseIf);
PHP_METHOD(Stub_Statements, testElseIf1);
PHP_METHOD(Stub_Statements, testElseIf2);
PHP_METHOD(Stub_Statements, test544Issue);
PHP_METHOD(Stub_Statements, test544IssueWithVariable);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_statements_testpropertyacccessavoidtmpreuse, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_statements_testelseif, 0, 0, 1)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, num, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, num)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_statements_testelseif1, 0, 0, 1)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, num, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, num)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_statements_testelseif2, 0, 0, 2)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, num, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, num)
#endif
	ZEND_ARG_INFO(0, total)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_statements_test544issue, 0, 0, 1)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, step, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, step)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_statements_test544issuewithvariable, 0, 0, 1)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, step, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, step)
#endif
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_statements_method_entry) {
	PHP_ME(Stub_Statements, testPropertyAcccessAvoidTmpReuse, arginfo_stub_statements_testpropertyacccessavoidtmpreuse, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Statements, testElseIf, arginfo_stub_statements_testelseif, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Statements, testElseIf1, arginfo_stub_statements_testelseif1, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Statements, testElseIf2, arginfo_stub_statements_testelseif2, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Statements, test544Issue, arginfo_stub_statements_test544issue, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Statements, test544IssueWithVariable, arginfo_stub_statements_test544issuewithvariable, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
