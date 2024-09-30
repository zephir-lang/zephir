
extern zend_class_entry *stub_constants_ce;

ZEPHIR_INIT_CLASS(Stub_Constants);

PHP_METHOD(Stub_Constants, getPropWsVarsGet);
PHP_METHOD(Stub_Constants, getPropertyC1);
PHP_METHOD(Stub_Constants, getPropertyC2);
PHP_METHOD(Stub_Constants, getPropertyC3);
PHP_METHOD(Stub_Constants, getPropertyC4);
PHP_METHOD(Stub_Constants, getPropertyC5);
PHP_METHOD(Stub_Constants, getPropertyC6);
PHP_METHOD(Stub_Constants, getPropertyC7);
PHP_METHOD(Stub_Constants, getPropertyC8);
PHP_METHOD(Stub_Constants, getPropertyC9);
PHP_METHOD(Stub_Constants, testReadConstant);
PHP_METHOD(Stub_Constants, testReadClassConstant1);
PHP_METHOD(Stub_Constants, testReadClassConstant2);
PHP_METHOD(Stub_Constants, testReadClassConstant3);
PHP_METHOD(Stub_Constants, testPHPVersionEnvConstant);
PHP_METHOD(Stub_Constants, testClassMagicConstant);
PHP_METHOD(Stub_Constants, testMethodMagicConstant);
PHP_METHOD(Stub_Constants, testFunctionMagicConstant);
PHP_METHOD(Stub_Constants, testNamespaceMagicConstant);
PHP_METHOD(Stub_Constants, testDirConstant);
PHP_METHOD(Stub_Constants, testPHPVersionEnvConstantInExpValue);
PHP_METHOD(Stub_Constants, testStringDelimiterConstantDoubleQuoted);
PHP_METHOD(Stub_Constants, testStringConstantWithVars);
PHP_METHOD(Stub_Constants, testStringPropertyWithVarsAssigned);
PHP_METHOD(Stub_Constants, testStringPropertyWithVarsGet);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_constants_getpropwsvarsget, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_constants_getpropertyc1, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_constants_getpropertyc2, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_constants_getpropertyc3, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_constants_getpropertyc4, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_constants_getpropertyc5, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_constants_getpropertyc6, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_constants_getpropertyc7, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_constants_getpropertyc8, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_constants_getpropertyc9, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_constants_testreadconstant, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_constants_testreadclassconstant1, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_constants_testreadclassconstant2, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_constants_testreadclassconstant3, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_constants_testphpversionenvconstant, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_constants_testclassmagicconstant, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_constants_testmethodmagicconstant, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_constants_testfunctionmagicconstant, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_constants_testnamespacemagicconstant, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_constants_testdirconstant, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_constants_testphpversionenvconstantinexpvalue, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_constants_teststringdelimiterconstantdoublequoted, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_constants_teststringconstantwithvars, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_constants_teststringpropertywithvarsassigned, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_constants_teststringpropertywithvarsget, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_constants_method_entry) {
PHP_ME(Stub_Constants, getPropWsVarsGet, arginfo_stub_constants_getpropwsvarsget, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Constants, getPropertyC1, arginfo_stub_constants_getpropertyc1, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Constants, getPropertyC2, arginfo_stub_constants_getpropertyc2, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Constants, getPropertyC3, arginfo_stub_constants_getpropertyc3, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Constants, getPropertyC4, arginfo_stub_constants_getpropertyc4, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Constants, getPropertyC5, arginfo_stub_constants_getpropertyc5, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Constants, getPropertyC6, arginfo_stub_constants_getpropertyc6, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Constants, getPropertyC7, arginfo_stub_constants_getpropertyc7, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Constants, getPropertyC8, arginfo_stub_constants_getpropertyc8, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Constants, getPropertyC9, arginfo_stub_constants_getpropertyc9, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Constants, testReadConstant, arginfo_stub_constants_testreadconstant, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Constants, testReadClassConstant1, arginfo_stub_constants_testreadclassconstant1, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Constants, testReadClassConstant2, arginfo_stub_constants_testreadclassconstant2, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Constants, testReadClassConstant3, arginfo_stub_constants_testreadclassconstant3, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Constants, testPHPVersionEnvConstant, arginfo_stub_constants_testphpversionenvconstant, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Constants, testClassMagicConstant, arginfo_stub_constants_testclassmagicconstant, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Constants, testMethodMagicConstant, arginfo_stub_constants_testmethodmagicconstant, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Constants, testFunctionMagicConstant, arginfo_stub_constants_testfunctionmagicconstant, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Constants, testNamespaceMagicConstant, arginfo_stub_constants_testnamespacemagicconstant, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Constants, testDirConstant, arginfo_stub_constants_testdirconstant, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Constants, testPHPVersionEnvConstantInExpValue, arginfo_stub_constants_testphpversionenvconstantinexpvalue, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Constants, testStringDelimiterConstantDoubleQuoted, arginfo_stub_constants_teststringdelimiterconstantdoublequoted, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Constants, testStringConstantWithVars, arginfo_stub_constants_teststringconstantwithvars, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Constants, testStringPropertyWithVarsAssigned, arginfo_stub_constants_teststringpropertywithvarsassigned, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Constants, testStringPropertyWithVarsGet, arginfo_stub_constants_teststringpropertywithvarsget, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
