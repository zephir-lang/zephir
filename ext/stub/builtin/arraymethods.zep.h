
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
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_BuiltIn_ArrayMethods, getJoin1, arginfo_stub_builtin_arraymethods_getjoin1, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_BuiltIn_ArrayMethods, getJoin1, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_BuiltIn_ArrayMethods, getReversed1, arginfo_stub_builtin_arraymethods_getreversed1, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_BuiltIn_ArrayMethods, getReversed1, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_BuiltIn_ArrayMethods, getMap1, arginfo_stub_builtin_arraymethods_getmap1, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_BuiltIn_ArrayMethods, getMap1, NULL, ZEND_ACC_PUBLIC)
#endif
	PHP_FE_END
};
