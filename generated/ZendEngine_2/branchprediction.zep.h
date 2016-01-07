
extern zend_class_entry *test_branchprediction_ce;

ZEPHIR_INIT_CLASS(Test_BranchPrediction);

PHP_METHOD(Test_BranchPrediction, testLikely1);
PHP_METHOD(Test_BranchPrediction, testLikely2);
PHP_METHOD(Test_BranchPrediction, testUnlikely1);
PHP_METHOD(Test_BranchPrediction, testUnlikely2);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_branchprediction_testlikely2, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_branchprediction_testunlikely2, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_branchprediction_method_entry) {
	PHP_ME(Test_BranchPrediction, testLikely1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_BranchPrediction, testLikely2, arginfo_test_branchprediction_testlikely2, ZEND_ACC_PUBLIC)
	PHP_ME(Test_BranchPrediction, testUnlikely1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_BranchPrediction, testUnlikely2, arginfo_test_branchprediction_testunlikely2, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
