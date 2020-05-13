
extern zend_class_entry *stub_optimizers_arraymerge_ce;

ZEPHIR_INIT_CLASS(Stub_Optimizers_ArrayMerge);

PHP_METHOD(Stub_Optimizers_ArrayMerge, mergeTwoRequiredArrays);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_arraymerge_mergetworequiredarrays, 0, 0, 2)
	ZEND_ARG_ARRAY_INFO(0, arr1, 0)
	ZEND_ARG_ARRAY_INFO(0, arr2, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_optimizers_arraymerge_method_entry) {
	PHP_ME(Stub_Optimizers_ArrayMerge, mergeTwoRequiredArrays, arginfo_stub_optimizers_arraymerge_mergetworequiredarrays, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_FE_END
};
