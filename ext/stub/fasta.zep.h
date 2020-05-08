
extern zend_class_entry *stub_fasta_ce;

ZEPHIR_INIT_CLASS(Stub_Fasta);

PHP_METHOD(Stub_Fasta, fastaRepeat);
PHP_METHOD(Stub_Fasta, fastRandom);
PHP_METHOD(Stub_Fasta, main);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_fasta_fastarepeat, 0, 0, 2)
	ZEND_ARG_INFO(0, n)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, seq, IS_STRING, 0)
#else
	ZEND_ARG_INFO(0, seq)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_fasta_main, 0, 0, 1)
	ZEND_ARG_INFO(0, n)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_fasta_method_entry) {
	PHP_ME(Stub_Fasta, fastaRepeat, arginfo_stub_fasta_fastarepeat, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Fasta, fastRandom, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Fasta, main, arginfo_stub_fasta_main, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
