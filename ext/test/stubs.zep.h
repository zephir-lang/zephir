
extern zend_class_entry *test_stubs_ce;

ZEPHIR_INIT_CLASS(Test_Stubs);

PHP_METHOD(Test_Stubs, testDockBlockAndReturnType);
PHP_METHOD(Test_Stubs, testDocBlockAndReturnTypeDeclared);
PHP_METHOD(Test_Stubs, testMixedInputParamsDocBlock);
PHP_METHOD(Test_Stubs, testMixedInputParamsDocBlockDeclared);

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_stubs_testdockblockandreturntype, 0, 0, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_stubs_testdockblockandreturntype, 0, 0, IS_STRING, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_stubs_testdocblockandreturntypedeclared, 0, 0, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_stubs_testdocblockandreturntypedeclared, 0, 0, IS_STRING, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_stubs_testmixedinputparamsdocblock, 0, 1, IS_LONG, 1)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_stubs_testmixedinputparamsdocblock, 0, 1, IS_LONG, NULL, 1)
#endif
	ZEND_ARG_INFO(0, intOrString)
	ZEND_ARG_TYPE_INFO(0, number, IS_LONG, 0)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_stubs_testmixedinputparamsdocblockdeclared, 0, 1, IS_LONG, 1)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_stubs_testmixedinputparamsdocblockdeclared, 0, 1, IS_LONG, NULL, 1)
#endif
	ZEND_ARG_INFO(0, intOrString)
	ZEND_ARG_TYPE_INFO(0, number, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_stubs_method_entry) {
	PHP_ME(Test_Stubs, testDockBlockAndReturnType, arginfo_test_stubs_testdockblockandreturntype, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Stubs, testDocBlockAndReturnTypeDeclared, arginfo_test_stubs_testdocblockandreturntypedeclared, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Stubs, testMixedInputParamsDocBlock, arginfo_test_stubs_testmixedinputparamsdocblock, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Stubs, testMixedInputParamsDocBlockDeclared, arginfo_test_stubs_testmixedinputparamsdocblockdeclared, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
