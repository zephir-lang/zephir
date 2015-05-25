
extern zend_class_entry *test_regexdna_ce;

ZEPHIR_INIT_CLASS(Test_RegexDNA);

PHP_METHOD(Test_RegexDNA, process);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_regexdna_process, 0, 0, 1)
	ZEND_ARG_INFO(0, path)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_regexdna_method_entry) {
	PHP_ME(Test_RegexDNA, process, arginfo_test_regexdna_process, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
