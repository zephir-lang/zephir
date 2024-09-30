
extern zend_class_entry *stub_openssl_ce;

ZEPHIR_INIT_CLASS(Stub_Openssl);

PHP_METHOD(Stub_Openssl, randomPseudoBytes);
PHP_METHOD(Stub_Openssl, randomPseudoBytesLoop);
PHP_METHOD(Stub_Openssl, opensslEncrypt);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_openssl_randompseudobytes, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, length, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_openssl_randompseudobytesloop, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_openssl_opensslencrypt, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, data, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_openssl_method_entry) {
	PHP_ME(Stub_Openssl, randomPseudoBytes, arginfo_stub_openssl_randompseudobytes, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Openssl, randomPseudoBytesLoop, arginfo_stub_openssl_randompseudobytesloop, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Openssl, opensslEncrypt, arginfo_stub_openssl_opensslencrypt, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
