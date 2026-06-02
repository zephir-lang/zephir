
extern zend_class_entry *stub_issue2564_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2564);

PHP_METHOD(Stub_Issue2564, __construct);
PHP_METHOD(Stub_Issue2564, getCipher);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2564___construct, 0, 0, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, cipher, IS_STRING, 0, "'aes-256-cfb'")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, useSigning, _IS_BOOL, 0, "true")
	ZEND_ARG_OBJ_TYPE_MASK(0, padFactory, Stub\\Issue2564PadFactory, MAY_BE_NULL, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2564_getcipher, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2564_method_entry) {
	PHP_ME(Stub_Issue2564, __construct, arginfo_stub_issue2564___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Stub_Issue2564, getCipher, arginfo_stub_issue2564_getcipher, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
