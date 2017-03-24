
extern zend_class_entry *test_oo_abstractstatic_ce;

ZEPHIR_INIT_CLASS(Test_Oo_AbstractStatic);

PHP_METHOD(Test_Oo_AbstractStatic, getCalledClass);

ZEPHIR_INIT_FUNCS(test_oo_abstractstatic_method_entry) {
	PHP_ME(Test_Oo_AbstractStatic, getCalledClass, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
