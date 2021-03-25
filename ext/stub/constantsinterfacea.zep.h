
extern zend_class_entry *stub_constantsinterfacea_ce;

ZEPHIR_INIT_CLASS(Stub_ConstantsInterfaceA);

PHP_METHOD(Stub_ConstantsInterfaceA, testReadInterfaceConstant1);
PHP_METHOD(Stub_ConstantsInterfaceA, testReadInterfaceConstant2);
PHP_METHOD(Stub_ConstantsInterfaceA, testReadInterfaceConstant3);
PHP_METHOD(Stub_ConstantsInterfaceA, testReadInterfaceConstant4);
PHP_METHOD(Stub_ConstantsInterfaceA, testReadInterfaceConstant5);
PHP_METHOD(Stub_ConstantsInterfaceA, testReadInterfaceConstant6);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_constantsinterfacea_testreadinterfaceconstant1, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_constantsinterfacea_testreadinterfaceconstant2, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_constantsinterfacea_testreadinterfaceconstant3, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_constantsinterfacea_testreadinterfaceconstant4, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_constantsinterfacea_testreadinterfaceconstant5, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_constantsinterfacea_testreadinterfaceconstant6, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_constantsinterfacea_method_entry) {
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_ConstantsInterfaceA, testReadInterfaceConstant1, arginfo_stub_constantsinterfacea_testreadinterfaceconstant1, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_ConstantsInterfaceA, testReadInterfaceConstant1, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_ConstantsInterfaceA, testReadInterfaceConstant2, arginfo_stub_constantsinterfacea_testreadinterfaceconstant2, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_ConstantsInterfaceA, testReadInterfaceConstant2, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_ConstantsInterfaceA, testReadInterfaceConstant3, arginfo_stub_constantsinterfacea_testreadinterfaceconstant3, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_ConstantsInterfaceA, testReadInterfaceConstant3, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_ConstantsInterfaceA, testReadInterfaceConstant4, arginfo_stub_constantsinterfacea_testreadinterfaceconstant4, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_ConstantsInterfaceA, testReadInterfaceConstant4, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_ConstantsInterfaceA, testReadInterfaceConstant5, arginfo_stub_constantsinterfacea_testreadinterfaceconstant5, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_ConstantsInterfaceA, testReadInterfaceConstant5, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_ConstantsInterfaceA, testReadInterfaceConstant6, arginfo_stub_constantsinterfacea_testreadinterfaceconstant6, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_ConstantsInterfaceA, testReadInterfaceConstant6, NULL, ZEND_ACC_PUBLIC)
#endif
	PHP_FE_END
};
