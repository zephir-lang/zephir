
extern zend_class_entry *test_declare_ce;

ZEPHIR_INIT_CLASS(Test_Declare);

PHP_METHOD(Test_Declare, testDeclare1);

ZEPHIR_INIT_FUNCS(test_declare_method_entry) {
	PHP_ME(Test_Declare, testDeclare1, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
