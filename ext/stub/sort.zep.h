
extern zend_class_entry *stub_sort_ce;

ZEPHIR_INIT_CLASS(Stub_Sort);

PHP_METHOD(Stub_Sort, quick);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_sort_quick, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, arr, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_sort_method_entry) {
	PHP_ME(Stub_Sort, quick, arginfo_stub_sort_quick, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
