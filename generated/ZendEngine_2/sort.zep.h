
extern zend_class_entry *test_sort_ce;

ZEPHIR_INIT_CLASS(Test_Sort);

PHP_METHOD(Test_Sort, quick);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_sort_quick, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, arr, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_sort_method_entry) {
	PHP_ME(Test_Sort, quick, arginfo_test_sort_quick, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
