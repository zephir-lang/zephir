
extern zend_class_entry *stub_issue2774_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2774);

PHP_METHOD(Stub_Issue2774, exportCast);
PHP_METHOD(Stub_Issue2774, exportNative);
PHP_METHOD(Stub_Issue2774, exportInt);
PHP_METHOD(Stub_Issue2774, exportDouble);
PHP_METHOD(Stub_Issue2774, exportSum);
PHP_METHOD(Stub_Issue2774, exportLiteral);
PHP_METHOD(Stub_Issue2774, exportShadowedLiteral);
PHP_METHOD(Stub_Issue2774, dumpShadowedLiteral);
PHP_METHOD(Stub_Issue2774, exportVar);
PHP_METHOD(Stub_Issue2774, exportArray);
PHP_METHOD(Stub_Issue2774, exportString);
PHP_METHOD(Stub_Issue2774, exportConcat);
PHP_METHOD(Stub_Issue2774, printReturned);
PHP_METHOD(Stub_Issue2774, printFalse);
PHP_METHOD(Stub_Issue2774, printStatement);
PHP_METHOD(Stub_Issue2774, discarded);
PHP_METHOD(Stub_Issue2774, exportFlag);
PHP_METHOD(Stub_Issue2774, echoExport);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2774_exportcast, 0, 1, IS_STRING, 0)
	ZEND_ARG_INFO(0, flag)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2774_exportnative, 0, 1, IS_STRING, 0)
	ZEND_ARG_INFO(0, flag)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2774_exportint, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, value, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2774_exportdouble, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, value, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2774_exportsum, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2774_exportliteral, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2774_exportshadowedliteral, 0, 0, 1)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2774_dumpshadowedliteral, 0, 0, 1)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2774_exportvar, 0, 0, 1)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2774_exportarray, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, value, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2774_exportstring, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, value, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2774_exportconcat, 0, 0, 1)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2774_printreturned, 0, 0, 1)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2774_printfalse, 0, 0, 1)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2774_printstatement, 0, 0, 1)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2774_discarded, 0, 0, 1)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2774_exportflag, 0, 0, 2)
	ZEND_ARG_INFO(0, value)
	ZEND_ARG_INFO(0, flag)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2774_echoexport, 0, 0, 1)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2774_method_entry) {
	PHP_ME(Stub_Issue2774, exportCast, arginfo_stub_issue2774_exportcast, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2774, exportNative, arginfo_stub_issue2774_exportnative, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2774, exportInt, arginfo_stub_issue2774_exportint, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2774, exportDouble, arginfo_stub_issue2774_exportdouble, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2774, exportSum, arginfo_stub_issue2774_exportsum, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Issue2774, exportLiteral, arginfo_stub_issue2774_exportliteral, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2774, exportShadowedLiteral, arginfo_stub_issue2774_exportshadowedliteral, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2774, dumpShadowedLiteral, arginfo_stub_issue2774_dumpshadowedliteral, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2774, exportVar, arginfo_stub_issue2774_exportvar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2774, exportArray, arginfo_stub_issue2774_exportarray, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2774, exportString, arginfo_stub_issue2774_exportstring, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2774, exportConcat, arginfo_stub_issue2774_exportconcat, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2774, printReturned, arginfo_stub_issue2774_printreturned, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2774, printFalse, arginfo_stub_issue2774_printfalse, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2774, printStatement, arginfo_stub_issue2774_printstatement, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2774, discarded, arginfo_stub_issue2774_discarded, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2774, exportFlag, arginfo_stub_issue2774_exportflag, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2774, echoExport, arginfo_stub_issue2774_echoexport, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
