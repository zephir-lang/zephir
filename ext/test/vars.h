
extern zend_class_entry *test_vars_ce;

ZEPHIR_INIT_CLASS(Test_Vars);

PHP_METHOD(Test_Vars, testVarDump);
PHP_METHOD(Test_Vars, testVarExport);

ZEPHIR_INIT_FUNCS(test_vars_method_entry) {
	PHP_ME(Test_Vars, testVarDump, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Vars, testVarExport, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
