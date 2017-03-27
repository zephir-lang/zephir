
extern zend_class_entry *test_instance_ce;

ZEPHIR_INIT_CLASS(Test_Instance);

PHP_METHOD(Test_Instance, __construct);
PHP_METHOD(Test_Instance, testIssue1339);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_instance___construct, 0, 0, 11)
	ZEND_ARG_OBJ_INFO(0, a1, Test\\Arithmetic, 0)
	ZEND_ARG_OBJ_INFO(0, a2, Test\\ArrayObject, 0)
	ZEND_ARG_OBJ_INFO(0, a3, Test\\Assign, 0)
	ZEND_ARG_OBJ_INFO(0, a4, Test\\Bitwise, 0)
	ZEND_ARG_OBJ_INFO(0, a5, Test\\BranchPrediction, 0)
	ZEND_ARG_OBJ_INFO(0, a6, Test\\Cast, 0)
	ZEND_ARG_OBJ_INFO(0, a7, Test\\Cblock, 0)
	ZEND_ARG_OBJ_INFO(0, a8, Test\\Chars, 0)
	ZEND_ARG_OBJ_INFO(0, a9, Test\\Closures, 0)
	ZEND_ARG_OBJ_INFO(0, a10, Test\\Compare, 0)
	ZEND_ARG_OBJ_INFO(0, a11, Test\\Concat, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_instance_method_entry) {
	PHP_ME(Test_Instance, __construct, arginfo_test_instance___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Test_Instance, testIssue1339, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
