
extern zend_class_entry *stub_quantum_ce;

ZEPHIR_INIT_CLASS(Stub_Quantum);

PHP_METHOD(Stub_Quantum, harmos);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_quantum_harmos, 0, 0, 1)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, x, IS_DOUBLE, 0)
#else
	ZEND_ARG_INFO(0, x)
#endif
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_quantum_method_entry) {
	PHP_ME(Stub_Quantum, harmos, arginfo_stub_quantum_harmos, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
