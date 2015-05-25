
extern zend_class_entry *test_constantsinterface_ce;

ZEPHIR_INIT_CLASS(Test_ConstantsInterface);

PHP_METHOD(Test_ConstantsInterface, testReadInterfaceConstant1);
PHP_METHOD(Test_ConstantsInterface, testReadInterfaceConstant2);
PHP_METHOD(Test_ConstantsInterface, testReadInterfaceConstant3);
PHP_METHOD(Test_ConstantsInterface, testReadInterfaceConstant4);
PHP_METHOD(Test_ConstantsInterface, testReadInterfaceConstant5);
PHP_METHOD(Test_ConstantsInterface, testReadInterfaceConstant6);

ZEPHIR_INIT_FUNCS(test_constantsinterface_method_entry) {
	PHP_ME(Test_ConstantsInterface, testReadInterfaceConstant1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_ConstantsInterface, testReadInterfaceConstant2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_ConstantsInterface, testReadInterfaceConstant3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_ConstantsInterface, testReadInterfaceConstant4, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_ConstantsInterface, testReadInterfaceConstant5, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_ConstantsInterface, testReadInterfaceConstant6, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
