
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
	PHP_FE_END
};
