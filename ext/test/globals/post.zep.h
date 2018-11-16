
extern zend_class_entry *test_globals_post_ce;

ZEPHIR_INIT_CLASS(Test_Globals_Post);

PHP_METHOD(Test_Globals_Post, hasValue);

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_globals_post_hasvalue, 0, 1, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_globals_post_hasvalue, 0, 1, _IS_BOOL, NULL, 0)
#endif
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_globals_post_method_entry) {
	PHP_ME(Test_Globals_Post, hasValue, arginfo_test_globals_post_hasvalue, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
