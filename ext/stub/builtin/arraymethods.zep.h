
extern zend_class_entry *stub_builtin_arraymethods_ce;

ZEPHIR_INIT_CLASS(Stub_BuiltIn_ArrayMethods);

PHP_METHOD(Stub_BuiltIn_ArrayMethods, getJoin1);
PHP_METHOD(Stub_BuiltIn_ArrayMethods, getReversed1);
PHP_METHOD(Stub_BuiltIn_ArrayMethods, getMap1);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_arraymethods_getjoin1, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_arraymethods_getreversed1, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_arraymethods_getmap1, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_builtin_arraymethods_method_entry) {
	PHP_ME(Stub_BuiltIn_ArrayMethods, getJoin1, arginfo_stub_builtin_arraymethods_getjoin1, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BuiltIn_ArrayMethods, getReversed1, arginfo_stub_builtin_arraymethods_getreversed1, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BuiltIn_ArrayMethods, getMap1, arginfo_stub_builtin_arraymethods_getmap1, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
