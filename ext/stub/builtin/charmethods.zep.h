
extern zend_class_entry *stub_builtin_charmethods_ce;

ZEPHIR_INIT_CLASS(Stub_BuiltIn_CharMethods);

PHP_METHOD(Stub_BuiltIn_CharMethods, getHex);
PHP_METHOD(Stub_BuiltIn_CharMethods, getHexForString);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_charmethods_gethex, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_charmethods_gethexforstring, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_builtin_charmethods_method_entry) {
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_BuiltIn_CharMethods, getHex, arginfo_stub_builtin_charmethods_gethex, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_BuiltIn_CharMethods, getHex, NULL, ZEND_ACC_PUBLIC)
#endif
	PHP_ME(Stub_BuiltIn_CharMethods, getHexForString, arginfo_stub_builtin_charmethods_gethexforstring, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
