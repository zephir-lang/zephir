
extern zend_class_entry *stub_builtin_charmethods_ce;

ZEPHIR_INIT_CLASS(Stub_BuiltIn_CharMethods);

PHP_METHOD(Stub_BuiltIn_CharMethods, getHex);
PHP_METHOD(Stub_BuiltIn_CharMethods, getHexForString);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_charmethods_gethexforstring, 0, 0, 1)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
#else
	ZEND_ARG_INFO(0, str)
#endif
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_builtin_charmethods_method_entry) {
	PHP_ME(Stub_BuiltIn_CharMethods, getHex, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BuiltIn_CharMethods, getHexForString, arginfo_stub_builtin_charmethods_gethexforstring, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
