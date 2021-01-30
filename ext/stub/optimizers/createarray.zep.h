
extern zend_class_entry *stub_optimizers_createarray_ce;

ZEPHIR_INIT_CLASS(Stub_Optimizers_CreateArray);

PHP_METHOD(Stub_Optimizers_CreateArray, createNoSize);
PHP_METHOD(Stub_Optimizers_CreateArray, createSize);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_createarray_createnosize, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_createarray_createsize, 0, 0, 1)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, n)
#endif
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_optimizers_createarray_method_entry) {
	PHP_ME(Stub_Optimizers_CreateArray, createNoSize, arginfo_stub_optimizers_createarray_createnosize, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Optimizers_CreateArray, createSize, arginfo_stub_optimizers_createarray_createsize, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
