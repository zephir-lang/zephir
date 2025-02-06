
extern zend_class_entry *stub_stubs_ce;

ZEPHIR_INIT_CLASS(Stub_Stubs);

PHP_METHOD(Stub_Stubs, testDockBlockAndReturnType);
PHP_METHOD(Stub_Stubs, testDocBlockAndReturnTypeDeclared);
PHP_METHOD(Stub_Stubs, testMixedInputParamsDocBlock);
PHP_METHOD(Stub_Stubs, testMixedInputParamsDocBlockDeclared);
PHP_METHOD(Stub_Stubs, testVariableLength);
PHP_METHOD(Stub_Stubs, testMultiReturnTypeWithMixedInDocAndEmptyInSignature1);
PHP_METHOD(Stub_Stubs, testMultiReturnTypeWithMixedInDocAndEmptyInSignature2);
PHP_METHOD(Stub_Stubs, testMultiReturnTypeWithBool);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_stubs_testdockblockandreturntype, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_stubs_testdocblockandreturntypedeclared, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_stubs_testmixedinputparamsdocblock, 0, 1, IS_LONG, 1)
	ZEND_ARG_INFO(0, intOrString)
	ZEND_ARG_TYPE_INFO(0, number, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_stubs_testmixedinputparamsdocblockdeclared, 0, 1, IS_LONG, 1)
	ZEND_ARG_INFO(0, intOrString)
	ZEND_ARG_TYPE_INFO(0, number, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_stubs_testvariablelength, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_stubs_testmultireturntypewithmixedindocandemptyinsignature1, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_stubs_testmultireturntypewithmixedindocandemptyinsignature2, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_stubs_testmultireturntypewithbool, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_stubs_method_entry) {
	PHP_ME(Stub_Stubs, testDockBlockAndReturnType, arginfo_stub_stubs_testdockblockandreturntype, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Stubs, testDocBlockAndReturnTypeDeclared, arginfo_stub_stubs_testdocblockandreturntypedeclared, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Stubs, testMixedInputParamsDocBlock, arginfo_stub_stubs_testmixedinputparamsdocblock, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Stubs, testMixedInputParamsDocBlockDeclared, arginfo_stub_stubs_testmixedinputparamsdocblockdeclared, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Stubs, testVariableLength, arginfo_stub_stubs_testvariablelength, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Stubs, testMultiReturnTypeWithMixedInDocAndEmptyInSignature1, arginfo_stub_stubs_testmultireturntypewithmixedindocandemptyinsignature1, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Stubs, testMultiReturnTypeWithMixedInDocAndEmptyInSignature2, arginfo_stub_stubs_testmultireturntypewithmixedindocandemptyinsignature2, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Stubs, testMultiReturnTypeWithBool, arginfo_stub_stubs_testmultireturntypewithbool, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
