
extern zend_class_entry *stub_regexdna_ce;

ZEPHIR_INIT_CLASS(Stub_RegexDNA);

PHP_METHOD(Stub_RegexDNA, process);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_regexdna_process, 0, 0, 1)
	ZEND_ARG_INFO(0, path)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_regexdna_method_entry) {
	PHP_ME(Stub_RegexDNA, process, arginfo_stub_regexdna_process, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
