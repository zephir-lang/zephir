
extern zend_class_entry *stub_builtin_arraymethods_ce;

ZEPHIR_INIT_CLASS(Stub_BuiltIn_ArrayMethods);

PHP_METHOD(Stub_BuiltIn_ArrayMethods, getJoin1);
PHP_METHOD(Stub_BuiltIn_ArrayMethods, getReversed1);
PHP_METHOD(Stub_BuiltIn_ArrayMethods, getMap1);

ZEPHIR_INIT_FUNCS(stub_builtin_arraymethods_method_entry) {
	PHP_ME(Stub_BuiltIn_ArrayMethods, getJoin1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BuiltIn_ArrayMethods, getReversed1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BuiltIn_ArrayMethods, getMap1, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
