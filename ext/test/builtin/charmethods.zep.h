
extern zend_class_entry *test_builtin_charmethods_ce;

ZEPHIR_INIT_CLASS(Test_BuiltIn_CharMethods);

PHP_METHOD(Test_BuiltIn_CharMethods, getHex);
PHP_METHOD(Test_BuiltIn_CharMethods, getHexForString);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_builtin_charmethods_gethexforstring, 0, 0, 1)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_builtin_charmethods_method_entry) {
	PHP_ME(Test_BuiltIn_CharMethods, getHex, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_BuiltIn_CharMethods, getHexForString, arginfo_test_builtin_charmethods_gethexforstring, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
