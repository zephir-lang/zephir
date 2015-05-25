
extern zend_class_entry *test_functional_ce;

ZEPHIR_INIT_CLASS(Test_Functional);

PHP_METHOD(Test_Functional, map1);
PHP_METHOD(Test_Functional, map2);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_functional_map1, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, a, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_functional_map2, 0, 0, 2)
	ZEND_ARG_ARRAY_INFO(0, a, 0)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_functional_method_entry) {
	PHP_ME(Test_Functional, map1, arginfo_test_functional_map1, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Functional, map2, arginfo_test_functional_map2, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
