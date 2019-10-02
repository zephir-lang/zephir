
extern zend_class_entry *test_fortytwo_ce;

ZEPHIR_INIT_CLASS(Test_FortyTwo);

PHP_METHOD(Test_FortyTwo, proof);

#if PHP_VERSION_ID >= 70100
#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_fortytwo_proof, 0, 0, IS_VOID, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_fortytwo_proof, 0, 0, IS_VOID, NULL, 0)
#endif
ZEND_END_ARG_INFO()
#else
#define arginfo_test_fortytwo_proof NULL
#endif

ZEPHIR_INIT_FUNCS(test_fortytwo_method_entry) {
	PHP_ME(Test_FortyTwo, proof, arginfo_test_fortytwo_proof, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
