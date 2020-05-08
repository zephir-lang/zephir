
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

ZEPHIR_INIT_FUNCS(stub_constants_method_entry) {
	PHP_ME(Stub_Constants, getPropWsVarsGet, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Constants, getPropertyC1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Constants, getPropertyC2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Constants, getPropertyC3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Constants, getPropertyC4, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Constants, getPropertyC5, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Constants, getPropertyC6, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Constants, getPropertyC7, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Constants, getPropertyC8, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Constants, getPropertyC9, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Constants, testReadConstant, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Constants, testReadClassConstant1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Constants, testReadClassConstant2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Constants, testReadClassConstant3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Constants, testPHPVersionEnvConstant, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Constants, testClassMagicConstant, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Constants, testMethodMagicConstant, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Constants, testFunctionMagicConstant, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Constants, testNamespaceMagicConstant, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Constants, testDirConstant, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Constants, testPHPVersionEnvConstantInExpValue, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Constants, testStringDelimiterConstantDoubleQuoted, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Constants, testStringConstantWithVars, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Constants, testStringPropertyWithVarsAssigned, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Constants, testStringPropertyWithVarsGet, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
