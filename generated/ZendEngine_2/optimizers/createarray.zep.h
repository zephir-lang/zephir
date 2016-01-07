
extern zend_class_entry *test_optimizers_createarray_ce;

ZEPHIR_INIT_CLASS(Test_Optimizers_CreateArray);

PHP_METHOD(Test_Optimizers_CreateArray, createNoSize);
PHP_METHOD(Test_Optimizers_CreateArray, createSize);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_optimizers_createarray_createsize, 0, 0, 1)
	ZEND_ARG_INFO(0, n)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_optimizers_createarray_method_entry) {
	PHP_ME(Test_Optimizers_CreateArray, createNoSize, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Optimizers_CreateArray, createSize, arginfo_test_optimizers_createarray_createsize, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
