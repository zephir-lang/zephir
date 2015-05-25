
extern zend_class_entry *test_constantsinterfacea_ce;

ZEPHIR_INIT_CLASS(Test_ConstantsInterfaceA);

PHP_METHOD(Test_ConstantsInterfaceA, testReadInterfaceConstant1);
PHP_METHOD(Test_ConstantsInterfaceA, testReadInterfaceConstant2);
PHP_METHOD(Test_ConstantsInterfaceA, testReadInterfaceConstant3);
PHP_METHOD(Test_ConstantsInterfaceA, testReadInterfaceConstant4);
PHP_METHOD(Test_ConstantsInterfaceA, testReadInterfaceConstant5);
PHP_METHOD(Test_ConstantsInterfaceA, testReadInterfaceConstant6);

ZEPHIR_INIT_FUNCS(test_constantsinterfacea_method_entry) {
	PHP_ME(Test_ConstantsInterfaceA, testReadInterfaceConstant1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_ConstantsInterfaceA, testReadInterfaceConstant2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_ConstantsInterfaceA, testReadInterfaceConstant3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_ConstantsInterfaceA, testReadInterfaceConstant4, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_ConstantsInterfaceA, testReadInterfaceConstant5, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_ConstantsInterfaceA, testReadInterfaceConstant6, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
