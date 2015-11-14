
extern zend_class_entry *test_builtin_arraymethods_ce;

ZEPHIR_INIT_CLASS(Test_BuiltIn_ArrayMethods);

PHP_METHOD(Test_BuiltIn_ArrayMethods, getJoin1);
PHP_METHOD(Test_BuiltIn_ArrayMethods, getReversed1);
PHP_METHOD(Test_BuiltIn_ArrayMethods, getMap1);

ZEPHIR_INIT_FUNCS(test_builtin_arraymethods_method_entry) {
	PHP_ME(Test_BuiltIn_ArrayMethods, getJoin1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_BuiltIn_ArrayMethods, getReversed1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_BuiltIn_ArrayMethods, getMap1, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
