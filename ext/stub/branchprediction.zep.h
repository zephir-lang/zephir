
extern zend_class_entry *stub_branchprediction_ce;

ZEPHIR_INIT_CLASS(Stub_BranchPrediction);

PHP_METHOD(Stub_BranchPrediction, testLikely1);
PHP_METHOD(Stub_BranchPrediction, testLikely2);
PHP_METHOD(Stub_BranchPrediction, testUnlikely1);
PHP_METHOD(Stub_BranchPrediction, testUnlikely2);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_branchprediction_testlikely2, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_branchprediction_testunlikely2, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_branchprediction_method_entry) {
	PHP_ME(Stub_BranchPrediction, testLikely1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BranchPrediction, testLikely2, arginfo_stub_branchprediction_testlikely2, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BranchPrediction, testUnlikely1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BranchPrediction, testUnlikely2, arginfo_stub_branchprediction_testunlikely2, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
