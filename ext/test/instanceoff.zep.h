
extern zend_class_entry *test_instanceoff_ce;

ZEPHIR_INIT_CLASS(Test_Instanceoff);

PHP_METHOD(Test_Instanceoff, testInstanceOf1);
PHP_METHOD(Test_Instanceoff, testInstanceOf2);
PHP_METHOD(Test_Instanceoff, testInstanceOf3);
PHP_METHOD(Test_Instanceoff, testInstanceOf4);
PHP_METHOD(Test_Instanceoff, testInstanceOf5);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_instanceoff_testinstanceof4, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_instanceoff_testinstanceof5, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_instanceoff_method_entry) {
	PHP_ME(Test_Instanceoff, testInstanceOf1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Instanceoff, testInstanceOf2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Instanceoff, testInstanceOf3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Instanceoff, testInstanceOf4, arginfo_test_instanceoff_testinstanceof4, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Instanceoff, testInstanceOf5, arginfo_test_instanceoff_testinstanceof5, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
