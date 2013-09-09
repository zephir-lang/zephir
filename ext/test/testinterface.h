
extern zend_class_entry *test_testinterface_ce;

ZEPHIR_INIT_CLASS(Test_TestInterface);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_testinterface_setaction, 0, 0, 0)
	ZEND_ARG_INFO(0, action)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_testinterface_method_entry) {
	PHP_ABSTRACT_ME(Test_TestInterface, setAction, arginfo_test_testinterface_setaction, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
