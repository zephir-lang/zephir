
extern zend_class_entry *stub_constantsinterface_ce;

ZEPHIR_INIT_CLASS(Stub_ConstantsInterface);

PHP_METHOD(Stub_ConstantsInterface, testReadInterfaceConstant1);
PHP_METHOD(Stub_ConstantsInterface, testReadInterfaceConstant2);
PHP_METHOD(Stub_ConstantsInterface, testReadInterfaceConstant3);
PHP_METHOD(Stub_ConstantsInterface, testReadInterfaceConstant4);
PHP_METHOD(Stub_ConstantsInterface, testReadInterfaceConstant5);
PHP_METHOD(Stub_ConstantsInterface, testReadInterfaceConstant6);
PHP_METHOD(Stub_ConstantsInterface, testReadInheritanceFromInterfaceConstant1);
PHP_METHOD(Stub_ConstantsInterface, testReadInheritanceFromInterfaceConstant2);
PHP_METHOD(Stub_ConstantsInterface, testReadInheritanceFromInterfaceConstant3);
PHP_METHOD(Stub_ConstantsInterface, testReadInheritanceFromInterfaceConstant4);
PHP_METHOD(Stub_ConstantsInterface, testReadInheritanceFromInterfaceConstant5);
PHP_METHOD(Stub_ConstantsInterface, testReadInheritanceFromInterfaceConstant6);

ZEPHIR_INIT_FUNCS(stub_constantsinterface_method_entry) {
	PHP_ME(Stub_ConstantsInterface, testReadInterfaceConstant1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ConstantsInterface, testReadInterfaceConstant2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ConstantsInterface, testReadInterfaceConstant3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ConstantsInterface, testReadInterfaceConstant4, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ConstantsInterface, testReadInterfaceConstant5, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ConstantsInterface, testReadInterfaceConstant6, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ConstantsInterface, testReadInheritanceFromInterfaceConstant1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ConstantsInterface, testReadInheritanceFromInterfaceConstant2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ConstantsInterface, testReadInheritanceFromInterfaceConstant3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ConstantsInterface, testReadInheritanceFromInterfaceConstant4, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ConstantsInterface, testReadInheritanceFromInterfaceConstant5, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ConstantsInterface, testReadInheritanceFromInterfaceConstant6, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
