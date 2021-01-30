
extern zend_class_entry *stub_pregmatch_ce;

ZEPHIR_INIT_CLASS(Stub_Pregmatch);

PHP_METHOD(Stub_Pregmatch, testWithoutReturnAndMatches);
PHP_METHOD(Stub_Pregmatch, testWithoutReturns);
PHP_METHOD(Stub_Pregmatch, testWithoutMatches);
PHP_METHOD(Stub_Pregmatch, testPregMatchAll);
PHP_METHOD(Stub_Pregmatch, testPregMatchFallback);
PHP_METHOD(Stub_Pregmatch, testPregMatch2Params);
PHP_METHOD(Stub_Pregmatch, testPregMatch3Params);
PHP_METHOD(Stub_Pregmatch, testPregMatch4Params);
PHP_METHOD(Stub_Pregmatch, testPregMatch5Params);
PHP_METHOD(Stub_Pregmatch, testPregMatchSaveMatches);
PHP_METHOD(Stub_Pregmatch, testMatchAll);
PHP_METHOD(Stub_Pregmatch, testMatchAllInZep);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_pregmatch_testwithoutreturnandmatches, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_pregmatch_testwithoutreturns, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_pregmatch_testwithoutmatches, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_pregmatch_testpregmatchall, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_pregmatch_testpregmatchfallback, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_pregmatch_testpregmatch2params, 0, 0, 2)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, subject)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_pregmatch_testpregmatch3params, 0, 0, 3)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, subject)
	ZEND_ARG_INFO(0, matches)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_pregmatch_testpregmatch4params, 0, 0, 4)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, subject)
	ZEND_ARG_INFO(0, matches)
	ZEND_ARG_INFO(0, flags)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_pregmatch_testpregmatch5params, 0, 0, 5)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, subject)
	ZEND_ARG_INFO(0, matches)
	ZEND_ARG_INFO(0, flags)
	ZEND_ARG_INFO(0, offset)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_pregmatch_testpregmatchsavematches, 0, 0, 2)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
#else
	ZEND_ARG_INFO(0, str)
#endif
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, pattern, IS_STRING, 0)
#else
	ZEND_ARG_INFO(0, pattern)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_pregmatch_testmatchall, 0, 0, 1)
	ZEND_ARG_INFO(0, flags)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_pregmatch_testmatchallinzep, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_pregmatch_method_entry) {
	PHP_ME(Stub_Pregmatch, testWithoutReturnAndMatches, arginfo_stub_pregmatch_testwithoutreturnandmatches, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Pregmatch, testWithoutReturns, arginfo_stub_pregmatch_testwithoutreturns, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Pregmatch, testWithoutMatches, arginfo_stub_pregmatch_testwithoutmatches, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Pregmatch, testPregMatchAll, arginfo_stub_pregmatch_testpregmatchall, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Pregmatch, testPregMatchFallback, arginfo_stub_pregmatch_testpregmatchfallback, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Pregmatch, testPregMatch2Params, arginfo_stub_pregmatch_testpregmatch2params, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Pregmatch, testPregMatch3Params, arginfo_stub_pregmatch_testpregmatch3params, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Pregmatch, testPregMatch4Params, arginfo_stub_pregmatch_testpregmatch4params, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Pregmatch, testPregMatch5Params, arginfo_stub_pregmatch_testpregmatch5params, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Pregmatch, testPregMatchSaveMatches, arginfo_stub_pregmatch_testpregmatchsavematches, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Pregmatch, testMatchAll, arginfo_stub_pregmatch_testmatchall, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Pregmatch, testMatchAllInZep, arginfo_stub_pregmatch_testmatchallinzep, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
