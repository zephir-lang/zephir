
extern zend_class_entry *test_mcalldynamic_ce;

ZEPHIR_INIT_CLASS(Test_McallDynamic);

PHP_METHOD(Test_McallDynamic, testMethod1);
PHP_METHOD(Test_McallDynamic, testMagicCall1);
PHP_METHOD(Test_McallDynamic, __call);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_mcalldynamic___call, 0, 0, 2)
	ZEND_ARG_INFO(0, method)
	ZEND_ARG_INFO(0, arguments)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_mcalldynamic_method_entry) {
	PHP_ME(Test_McallDynamic, testMethod1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_McallDynamic, testMagicCall1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_McallDynamic, __call, arginfo_test_mcalldynamic___call, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
