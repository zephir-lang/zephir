
extern zend_class_entry *stub_oo_ooconstruct_ce;

ZEPHIR_INIT_CLASS(Stub_Oo_OoConstruct);

PHP_METHOD(Stub_Oo_OoConstruct, __construct);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_ooconstruct___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_oo_ooconstruct_method_entry) {
	PHP_ME(Stub_Oo_OoConstruct, __construct, arginfo_stub_oo_ooconstruct___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_FE_END
};
