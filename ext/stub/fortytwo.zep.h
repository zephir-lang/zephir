
extern zend_class_entry *stub_fortytwo_ce;

ZEPHIR_INIT_CLASS(Stub_FortyTwo);

PHP_METHOD(Stub_FortyTwo, proof);

#if PHP_VERSION_ID >= 70100
#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_fortytwo_proof, 0, 0, IS_VOID, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_fortytwo_proof, 0, 0, IS_VOID, NULL, 0)
#endif
ZEND_END_ARG_INFO()
#else
#define arginfo_stub_fortytwo_proof NULL
#endif

ZEPHIR_INIT_FUNCS(stub_fortytwo_method_entry) {
	PHP_ME(Stub_FortyTwo, proof, arginfo_stub_fortytwo_proof, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
