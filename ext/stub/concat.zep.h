
extern zend_class_entry *stub_concat_ce;

ZEPHIR_INIT_CLASS(Stub_Concat);

PHP_METHOD(Stub_Concat, getTestProperty);
PHP_METHOD(Stub_Concat, testConcatBySelfProperty);
PHP_METHOD(Stub_Concat, testConcat1);
PHP_METHOD(Stub_Concat, testConcat2);
PHP_METHOD(Stub_Concat, testConcat3);
PHP_METHOD(Stub_Concat, testConcat4);
PHP_METHOD(Stub_Concat, testConcat5);
PHP_METHOD(Stub_Concat, testConcatStringWithIntVar);
PHP_METHOD(Stub_Concat, testConcatStringWithLongVar);
PHP_METHOD(Stub_Concat, testConcatStringWithUintVar);
PHP_METHOD(Stub_Concat, testConcatStringWithUlongVar);
PHP_METHOD(Stub_Concat, testConcatStringWithCharVar);
PHP_METHOD(Stub_Concat, testConcatVarWithIntVar);
PHP_METHOD(Stub_Concat, testConcatVarWithCharVar);
PHP_METHOD(Stub_Concat, testConcatVarWithIntLiteral);
PHP_METHOD(Stub_Concat, testConcatStringWithDoubleVar);
PHP_METHOD(Stub_Concat, testConcatStringWithDoubleLiteral);
PHP_METHOD(Stub_Concat, testConcatStringWithBoolVar);
PHP_METHOD(Stub_Concat, testConcatStringWithTrueLiteral);
PHP_METHOD(Stub_Concat, testConcatStringWithFalseLiteral);
PHP_METHOD(Stub_Concat, testConcatVarWithDoubleVar);
PHP_METHOD(Stub_Concat, testConcatVarWithDoubleLiteral);
PHP_METHOD(Stub_Concat, testConcatVarWithBoolVar);
PHP_METHOD(Stub_Concat, testConcatVarWithTrueLiteral);
PHP_METHOD(Stub_Concat, testConcatVarWithFalseLiteral);
PHP_METHOD(Stub_Concat, testConcatIntVarWithFalseLiteral);
PHP_METHOD(Stub_Concat, testConcatVarWithNullLiteral);
PHP_METHOD(Stub_Concat, testConcatStringWithArrayLiteral);
PHP_METHOD(Stub_Concat, testConcatStringWithArrayVar);
PHP_METHOD(Stub_Concat, testConcatVarWithArrayLiteral);
PHP_METHOD(Stub_Concat, testConcatVarWithArrayVar);
PHP_METHOD(Stub_Concat, getStore);
PHP_METHOD(Stub_Concat, resetStore);
PHP_METHOD(Stub_Concat, appendPropertyLiterals);
PHP_METHOD(Stub_Concat, appendPropertyVariables);
PHP_METHOD(Stub_Concat, appendPropertyArray);
PHP_METHOD(Stub_Concat, getAppended);
PHP_METHOD(Stub_Concat, resetAppended);
PHP_METHOD(Stub_Concat, appendStaticLiterals);
PHP_METHOD(Stub_Concat, appendStaticVariables);
PHP_METHOD(Stub_Concat, testConcatExprWithBoolVar);
PHP_METHOD(Stub_Concat, testConcatExprWithTrueLiteral);
PHP_METHOD(Stub_Concat, testConcatExprWithFalseLiteral);
PHP_METHOD(Stub_Concat, testConcatExprWithCharVar);
PHP_METHOD(Stub_Concat, testConcatExprWithCharLiteral);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_gettestproperty, 0, 0, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_testconcatbyselfproperty, 0, 1, IS_VOID, 0)

	ZEND_ARG_TYPE_INFO(0, title, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_testconcat1, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_testconcat2, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_testconcat3, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_testconcat4, 0, 1, IS_STRING, 0)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_testconcat5, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, number, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_testconcatstringwithintvar, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, number, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_testconcatstringwithlongvar, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, number, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_testconcatstringwithuintvar, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_testconcatstringwithulongvar, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_testconcatstringwithcharvar, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_testconcatvarwithintvar, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, number, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_testconcatvarwithcharvar, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_testconcatvarwithintliteral, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_testconcatstringwithdoublevar, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, number, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_testconcatstringwithdoubleliteral, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_testconcatstringwithboolvar, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, flag, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_testconcatstringwithtrueliteral, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_testconcatstringwithfalseliteral, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_testconcatvarwithdoublevar, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, number, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_testconcatvarwithdoubleliteral, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_testconcatvarwithboolvar, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, flag, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_testconcatvarwithtrueliteral, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_testconcatvarwithfalseliteral, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_testconcatintvarwithfalseliteral, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_testconcatvarwithnullliteral, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_testconcatstringwitharrayliteral, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_testconcatstringwitharrayvar, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_testconcatvarwitharrayliteral, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_testconcatvarwitharrayvar, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_getstore, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_resetstore, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_appendpropertyliterals, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_appendpropertyvariables, 0, 4, IS_VOID, 0)

	ZEND_ARG_TYPE_INFO(0, number, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, fraction, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, flag, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, text, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_appendpropertyarray, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_getappended, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_resetappended, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_appendstaticliterals, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_appendstaticvariables, 0, 3, IS_VOID, 0)

	ZEND_ARG_TYPE_INFO(0, number, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, fraction, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, flag, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_testconcatexprwithboolvar, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, flag, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_testconcatexprwithtrueliteral, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_testconcatexprwithfalseliteral, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_testconcatexprwithcharvar, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_testconcatexprwithcharliteral, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_concat_method_entry) {
	PHP_ME(Stub_Concat, getTestProperty, arginfo_stub_concat_gettestproperty, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Concat, testConcatBySelfProperty, arginfo_stub_concat_testconcatbyselfproperty, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Concat, testConcat1, arginfo_stub_concat_testconcat1, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Concat, testConcat2, arginfo_stub_concat_testconcat2, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Concat, testConcat3, arginfo_stub_concat_testconcat3, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Concat, testConcat4, arginfo_stub_concat_testconcat4, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Concat, testConcat5, arginfo_stub_concat_testconcat5, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Concat, testConcatStringWithIntVar, arginfo_stub_concat_testconcatstringwithintvar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Concat, testConcatStringWithLongVar, arginfo_stub_concat_testconcatstringwithlongvar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Concat, testConcatStringWithUintVar, arginfo_stub_concat_testconcatstringwithuintvar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Concat, testConcatStringWithUlongVar, arginfo_stub_concat_testconcatstringwithulongvar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Concat, testConcatStringWithCharVar, arginfo_stub_concat_testconcatstringwithcharvar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Concat, testConcatVarWithIntVar, arginfo_stub_concat_testconcatvarwithintvar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Concat, testConcatVarWithCharVar, arginfo_stub_concat_testconcatvarwithcharvar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Concat, testConcatVarWithIntLiteral, arginfo_stub_concat_testconcatvarwithintliteral, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Concat, testConcatStringWithDoubleVar, arginfo_stub_concat_testconcatstringwithdoublevar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Concat, testConcatStringWithDoubleLiteral, arginfo_stub_concat_testconcatstringwithdoubleliteral, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Concat, testConcatStringWithBoolVar, arginfo_stub_concat_testconcatstringwithboolvar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Concat, testConcatStringWithTrueLiteral, arginfo_stub_concat_testconcatstringwithtrueliteral, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Concat, testConcatStringWithFalseLiteral, arginfo_stub_concat_testconcatstringwithfalseliteral, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Concat, testConcatVarWithDoubleVar, arginfo_stub_concat_testconcatvarwithdoublevar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Concat, testConcatVarWithDoubleLiteral, arginfo_stub_concat_testconcatvarwithdoubleliteral, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Concat, testConcatVarWithBoolVar, arginfo_stub_concat_testconcatvarwithboolvar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Concat, testConcatVarWithTrueLiteral, arginfo_stub_concat_testconcatvarwithtrueliteral, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Concat, testConcatVarWithFalseLiteral, arginfo_stub_concat_testconcatvarwithfalseliteral, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Concat, testConcatIntVarWithFalseLiteral, arginfo_stub_concat_testconcatintvarwithfalseliteral, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Concat, testConcatVarWithNullLiteral, arginfo_stub_concat_testconcatvarwithnullliteral, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Concat, testConcatStringWithArrayLiteral, arginfo_stub_concat_testconcatstringwitharrayliteral, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Concat, testConcatStringWithArrayVar, arginfo_stub_concat_testconcatstringwitharrayvar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Concat, testConcatVarWithArrayLiteral, arginfo_stub_concat_testconcatvarwitharrayliteral, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Concat, testConcatVarWithArrayVar, arginfo_stub_concat_testconcatvarwitharrayvar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Concat, getStore, arginfo_stub_concat_getstore, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Concat, resetStore, arginfo_stub_concat_resetstore, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Concat, appendPropertyLiterals, arginfo_stub_concat_appendpropertyliterals, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Concat, appendPropertyVariables, arginfo_stub_concat_appendpropertyvariables, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Concat, appendPropertyArray, arginfo_stub_concat_appendpropertyarray, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Concat, getAppended, arginfo_stub_concat_getappended, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Concat, resetAppended, arginfo_stub_concat_resetappended, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Concat, appendStaticLiterals, arginfo_stub_concat_appendstaticliterals, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Concat, appendStaticVariables, arginfo_stub_concat_appendstaticvariables, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Concat, testConcatExprWithBoolVar, arginfo_stub_concat_testconcatexprwithboolvar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Concat, testConcatExprWithTrueLiteral, arginfo_stub_concat_testconcatexprwithtrueliteral, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Concat, testConcatExprWithFalseLiteral, arginfo_stub_concat_testconcatexprwithfalseliteral, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Concat, testConcatExprWithCharVar, arginfo_stub_concat_testconcatexprwithcharvar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Concat, testConcatExprWithCharLiteral, arginfo_stub_concat_testconcatexprwithcharliteral, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
