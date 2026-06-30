
extern zend_class_entry *stub_constantparameterdefault_ce;

ZEPHIR_INIT_CLASS(Stub_ConstantParameterDefault);

PHP_METHOD(Stub_ConstantParameterDefault, __construct);
PHP_METHOD(Stub_ConstantParameterDefault, getCipher);
PHP_METHOD(Stub_ConstantParameterDefault, getRounds);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_constantparameterdefault___construct, 0, 0, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, cipher, IS_STRING, 0, "'aes-256-cfb'")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, rounds, IS_LONG, 0, "12")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_constantparameterdefault_getcipher, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_constantparameterdefault_getrounds, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_constantparameterdefault_method_entry) {
	PHP_ME(Stub_ConstantParameterDefault, __construct, arginfo_stub_constantparameterdefault___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Stub_ConstantParameterDefault, getCipher, arginfo_stub_constantparameterdefault_getcipher, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ConstantParameterDefault, getRounds, arginfo_stub_constantparameterdefault_getrounds, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
