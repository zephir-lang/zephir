
extern zend_class_entry *test_methodinterface_ce;

ZEPHIR_INIT_CLASS(Test_MethodInterface);

ZEPHIR_INIT_FUNCS(test_methodinterface_method_entry) {
	PHP_ABSTRACT_ME(Test_MethodInterface, testMethod, NULL)
	PHP_FE_END
};
