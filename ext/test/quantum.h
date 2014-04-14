
extern zend_class_entry *test_quantum_ce;

ZEPHIR_INIT_CLASS(Test_Quantum);

PHP_METHOD(Test_Quantum, Harmos);
PHP_METHOD(Test_Quantum, test);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_quantum_harmos, 0, 0, 1)
	ZEND_ARG_INFO(0, x)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_quantum_method_entry) {
	PHP_ME(Test_Quantum, Harmos, arginfo_test_quantum_harmos, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Quantum, test, NULL, ZEND_ACC_PUBLIC)
  PHP_FE_END
};
