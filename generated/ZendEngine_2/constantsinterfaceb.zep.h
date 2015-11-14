
extern zend_class_entry *test_constantsinterfaceb_ce;

ZEPHIR_INIT_CLASS(Test_ConstantsInterfaceB);

PHP_METHOD(Test_ConstantsInterfaceB, testReadInterfaceConstant1);
PHP_METHOD(Test_ConstantsInterfaceB, testReadInterfaceConstant2);
PHP_METHOD(Test_ConstantsInterfaceB, testReadInterfaceConstant3);
PHP_METHOD(Test_ConstantsInterfaceB, testReadInterfaceConstant4);
PHP_METHOD(Test_ConstantsInterfaceB, testReadInterfaceConstant5);
PHP_METHOD(Test_ConstantsInterfaceB, testReadInterfaceConstant6);

ZEPHIR_INIT_FUNCS(test_constantsinterfaceb_method_entry) {
	PHP_ME(Test_ConstantsInterfaceB, testReadInterfaceConstant1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_ConstantsInterfaceB, testReadInterfaceConstant2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_ConstantsInterfaceB, testReadInterfaceConstant3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_ConstantsInterfaceB, testReadInterfaceConstant4, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_ConstantsInterfaceB, testReadInterfaceConstant5, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_ConstantsInterfaceB, testReadInterfaceConstant6, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
