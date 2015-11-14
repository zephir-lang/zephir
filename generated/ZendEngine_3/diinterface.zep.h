
extern zend_class_entry *test_diinterface_ce;

ZEPHIR_INIT_CLASS(Test_DiInterface);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_diinterface_getshared, 0, 0, 1)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_diinterface_method_entry) {
	PHP_ABSTRACT_ME(Test_DiInterface, getShared, arginfo_test_diinterface_getshared)
	PHP_FE_END
};
