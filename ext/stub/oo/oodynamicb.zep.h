
extern zend_class_entry *stub_oo_oodynamicb_ce;

ZEPHIR_INIT_CLASS(Stub_Oo_OoDynamicB);

PHP_METHOD(Stub_Oo_OoDynamicB, execute);
PHP_METHOD(Stub_Oo_OoDynamicB, execute2);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_oodynamicb_execute, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_oodynamicb_execute2, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_oo_oodynamicb_method_entry) {
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Oo_OoDynamicB, execute, arginfo_stub_oo_oodynamicb_execute, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Oo_OoDynamicB, execute, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Oo_OoDynamicB, execute2, arginfo_stub_oo_oodynamicb_execute2, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Oo_OoDynamicB, execute2, NULL, ZEND_ACC_PUBLIC)
#endif
	PHP_FE_END
};
