
extern zend_class_entry *stub_oo_extendpdoclass_ce;

ZEPHIR_INIT_CLASS(Stub_Oo_ExtendPdoClass);

PHP_METHOD(Stub_Oo_ExtendPdoClass, __construct);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_extendpdoclass___construct, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, dsn, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, username, IS_STRING, 1)
	ZEND_ARG_TYPE_INFO(0, password, IS_STRING, 1)
	ZEND_ARG_INFO(0, attrs)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_oo_extendpdoclass_method_entry) {
	PHP_ME(Stub_Oo_ExtendPdoClass, __construct, arginfo_stub_oo_extendpdoclass___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_FE_END
};
