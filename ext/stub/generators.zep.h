
extern zend_class_entry *stub_generators_ce;

ZEPHIR_INIT_CLASS(Stub_Generators);

PHP_METHOD(Stub_Generators, simpleYield);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_generators_simpleyield, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_generators_method_entry) {
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Generators, simpleYield, arginfo_stub_generators_simpleyield, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Generators, simpleYield, NULL, ZEND_ACC_PUBLIC)
#endif
	PHP_FE_END
};
