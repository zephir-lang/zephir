
extern zend_class_entry *test_fannkuch_ce;

ZEPHIR_INIT_CLASS(Test_Fannkuch);

PHP_METHOD(Test_Fannkuch, process);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_fannkuch_process, 0, 0, 1)
	ZEND_ARG_INFO(0, n)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_fannkuch_method_entry) {
	PHP_ME(Test_Fannkuch, process, arginfo_test_fannkuch_process, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
