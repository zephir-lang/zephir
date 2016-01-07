
extern zend_class_entry *test_pregmatch_ce;

ZEPHIR_INIT_CLASS(Test_Pregmatch);

PHP_METHOD(Test_Pregmatch, testWithoutReturnAndMatches);
PHP_METHOD(Test_Pregmatch, testWithoutReturns);
PHP_METHOD(Test_Pregmatch, testWithoutMatches);
PHP_METHOD(Test_Pregmatch, testPregMatchAll);
PHP_METHOD(Test_Pregmatch, testPregMatchFallback);
PHP_METHOD(Test_Pregmatch, testPregMatch2Params);
PHP_METHOD(Test_Pregmatch, testPregMatch3Params);
PHP_METHOD(Test_Pregmatch, testPregMatch4Params);
PHP_METHOD(Test_Pregmatch, testPregMatch5Params);
PHP_METHOD(Test_Pregmatch, testPregMatchSaveMatches);
PHP_METHOD(Test_Pregmatch, testMatchAll);
PHP_METHOD(Test_Pregmatch, testMatchAllInZep);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_pregmatch_testpregmatch2params, 0, 0, 2)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, subject)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_pregmatch_testpregmatch3params, 0, 0, 3)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, subject)
	ZEND_ARG_INFO(0, matches)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_pregmatch_testpregmatch4params, 0, 0, 4)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, subject)
	ZEND_ARG_INFO(0, matches)
	ZEND_ARG_INFO(0, flags)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_pregmatch_testpregmatch5params, 0, 0, 5)
	ZEND_ARG_INFO(0, pattern)
	ZEND_ARG_INFO(0, subject)
	ZEND_ARG_INFO(0, matches)
	ZEND_ARG_INFO(0, flags)
	ZEND_ARG_INFO(0, offset)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_pregmatch_testpregmatchsavematches, 0, 0, 2)
	ZEND_ARG_INFO(0, str)
	ZEND_ARG_INFO(0, pattern)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_pregmatch_testmatchall, 0, 0, 1)
	ZEND_ARG_INFO(0, flags)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_pregmatch_method_entry) {
	PHP_ME(Test_Pregmatch, testWithoutReturnAndMatches, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Pregmatch, testWithoutReturns, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Pregmatch, testWithoutMatches, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Pregmatch, testPregMatchAll, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Pregmatch, testPregMatchFallback, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Pregmatch, testPregMatch2Params, arginfo_test_pregmatch_testpregmatch2params, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Pregmatch, testPregMatch3Params, arginfo_test_pregmatch_testpregmatch3params, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Pregmatch, testPregMatch4Params, arginfo_test_pregmatch_testpregmatch4params, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Pregmatch, testPregMatch5Params, arginfo_test_pregmatch_testpregmatch5params, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Pregmatch, testPregMatchSaveMatches, arginfo_test_pregmatch_testpregmatchsavematches, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Pregmatch, testMatchAll, arginfo_test_pregmatch_testmatchall, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Pregmatch, testMatchAllInZep, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
