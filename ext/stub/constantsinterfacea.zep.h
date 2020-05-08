
extern zend_class_entry *stub_constantsinterfacea_ce;

ZEPHIR_INIT_CLASS(Stub_ConstantsInterfaceA);

PHP_METHOD(Stub_ConstantsInterfaceA, testReadInterfaceConstant1);
PHP_METHOD(Stub_ConstantsInterfaceA, testReadInterfaceConstant2);
PHP_METHOD(Stub_ConstantsInterfaceA, testReadInterfaceConstant3);
PHP_METHOD(Stub_ConstantsInterfaceA, testReadInterfaceConstant4);
PHP_METHOD(Stub_ConstantsInterfaceA, testReadInterfaceConstant5);
PHP_METHOD(Stub_ConstantsInterfaceA, testReadInterfaceConstant6);

ZEPHIR_INIT_FUNCS(stub_constantsinterfacea_method_entry) {
	PHP_ME(Stub_ConstantsInterfaceA, testReadInterfaceConstant1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ConstantsInterfaceA, testReadInterfaceConstant2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ConstantsInterfaceA, testReadInterfaceConstant3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ConstantsInterfaceA, testReadInterfaceConstant4, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ConstantsInterfaceA, testReadInterfaceConstant5, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ConstantsInterfaceA, testReadInterfaceConstant6, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
