
extern zend_class_entry *test_oo_abstractclass_ce;

ZEPHIR_INIT_CLASS(Test_Oo_AbstractClass);

PHP_METHOD(Test_Oo_AbstractClass, test1);

ZEPHIR_INIT_FUNCS(test_oo_abstractclass_method_entry) {
	PHP_ME(Test_Oo_AbstractClass, test1, NULL, ZEND_ACC_ABSTRACT|ZEND_ACC_PUBLIC)
  PHP_FE_END
};
