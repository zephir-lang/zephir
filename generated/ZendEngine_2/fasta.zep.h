
extern zend_class_entry *test_fasta_ce;

ZEPHIR_INIT_CLASS(Test_Fasta);

PHP_METHOD(Test_Fasta, fastaRepeat);
PHP_METHOD(Test_Fasta, fastRandom);
PHP_METHOD(Test_Fasta, main);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_fasta_fastarepeat, 0, 0, 2)
	ZEND_ARG_INFO(0, n)
	ZEND_ARG_INFO(0, seq)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_fasta_main, 0, 0, 1)
	ZEND_ARG_INFO(0, n)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_fasta_method_entry) {
	PHP_ME(Test_Fasta, fastaRepeat, arginfo_test_fasta_fastarepeat, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Fasta, fastRandom, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Fasta, main, arginfo_test_fasta_main, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
