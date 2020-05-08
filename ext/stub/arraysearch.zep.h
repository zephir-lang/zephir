
extern zend_class_entry *stub_arraysearch_ce;

ZEPHIR_INIT_CLASS(Stub_ArraySearch);

PHP_METHOD(Stub_ArraySearch, simpleSearch);
PHP_METHOD(Stub_ArraySearch, searchUsingArrayInsideZephir);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_arraysearch_simplesearch, 0, 0, 2)
	ZEND_ARG_INFO(0, needle)
	ZEND_ARG_ARRAY_INFO(0, haystack, 0)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, strict, _IS_BOOL, 0)
#else
	ZEND_ARG_INFO(0, strict)
#endif
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_arraysearch_method_entry) {
	PHP_ME(Stub_ArraySearch, simpleSearch, arginfo_stub_arraysearch_simplesearch, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArraySearch, searchUsingArrayInsideZephir, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
