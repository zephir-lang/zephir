
extern zend_class_entry *test_arraysearch_ce;

ZEPHIR_INIT_CLASS(Test_ArraySearch);

PHP_METHOD(Test_ArraySearch, simpleSearch);
PHP_METHOD(Test_ArraySearch, searchUsingArrayInsideZephir);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_arraysearch_simplesearch, 0, 0, 2)
	ZEND_ARG_INFO(0, needle)
	ZEND_ARG_ARRAY_INFO(0, haystack, 0)
	ZEND_ARG_TYPE_INFO(0, strict, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_arraysearch_method_entry) {
	PHP_ME(Test_ArraySearch, simpleSearch, arginfo_test_arraysearch_simplesearch, ZEND_ACC_PUBLIC)
	PHP_ME(Test_ArraySearch, searchUsingArrayInsideZephir, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
