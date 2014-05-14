
extern zend_class_entry *test_instanceoff_ce;

ZEPHIR_INIT_CLASS(Test_Instanceoff);

PHP_METHOD(Test_Instanceoff, testInstanceOf1);
PHP_METHOD(Test_Instanceoff, testInstanceOf2);
PHP_METHOD(Test_Instanceoff, testInstanceOf3);

ZEPHIR_INIT_FUNCS(test_instanceoff_method_entry) {
	PHP_ME(Test_Instanceoff, testInstanceOf1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Instanceoff, testInstanceOf2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Instanceoff, testInstanceOf3, NULL, ZEND_ACC_PUBLIC)
  PHP_FE_END
};
