
extern zend_class_entry *test_requires_ce;

ZEPHIR_INIT_CLASS(Test_Requires);

PHP_METHOD(Test_Requires, requireExternal1);
PHP_METHOD(Test_Requires, requireExternal2);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_requires_requireexternal1, 0, 0, 1)
	ZEND_ARG_INFO(0, path)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_requires_requireexternal2, 0, 0, 1)
	ZEND_ARG_INFO(0, path)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_requires_method_entry) {
	PHP_ME(Test_Requires, requireExternal1, arginfo_test_requires_requireexternal1, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Requires, requireExternal2, arginfo_test_requires_requireexternal2, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
