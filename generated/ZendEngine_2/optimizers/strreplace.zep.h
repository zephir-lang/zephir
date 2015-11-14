
extern zend_class_entry *test_optimizers_strreplace_ce;

ZEPHIR_INIT_CLASS(Test_Optimizers_StrReplace);

PHP_METHOD(Test_Optimizers_StrReplace, issue1055);
PHP_METHOD(Test_Optimizers_StrReplace, issue1087);
PHP_METHOD(Test_Optimizers_StrReplace, issue732A);
PHP_METHOD(Test_Optimizers_StrReplace, issue732B);

ZEPHIR_INIT_FUNCS(test_optimizers_strreplace_method_entry) {
	PHP_ME(Test_Optimizers_StrReplace, issue1055, NULL, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Test_Optimizers_StrReplace, issue1087, NULL, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Test_Optimizers_StrReplace, issue732A, NULL, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Test_Optimizers_StrReplace, issue732B, NULL, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_FE_END
};
