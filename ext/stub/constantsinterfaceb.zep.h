
extern zend_class_entry *stub_constantsinterfaceb_ce;

ZEPHIR_INIT_CLASS(Stub_ConstantsInterfaceB);

PHP_METHOD(Stub_ConstantsInterfaceB, testReadInterfaceConstant1);
PHP_METHOD(Stub_ConstantsInterfaceB, testReadInterfaceConstant2);
PHP_METHOD(Stub_ConstantsInterfaceB, testReadInterfaceConstant3);
PHP_METHOD(Stub_ConstantsInterfaceB, testReadInterfaceConstant4);
PHP_METHOD(Stub_ConstantsInterfaceB, testReadInterfaceConstant5);
PHP_METHOD(Stub_ConstantsInterfaceB, testReadInterfaceConstant6);

ZEPHIR_INIT_FUNCS(stub_constantsinterfaceb_method_entry) {
	PHP_ME(Stub_ConstantsInterfaceB, testReadInterfaceConstant1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ConstantsInterfaceB, testReadInterfaceConstant2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ConstantsInterfaceB, testReadInterfaceConstant3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ConstantsInterfaceB, testReadInterfaceConstant4, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ConstantsInterfaceB, testReadInterfaceConstant5, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ConstantsInterfaceB, testReadInterfaceConstant6, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
