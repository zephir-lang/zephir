
extern zend_class_entry *stub_oo_oodynamica_ce;

ZEPHIR_INIT_CLASS(Stub_Oo_OoDynamicA);

PHP_METHOD(Stub_Oo_OoDynamicA, getNew);
PHP_METHOD(Stub_Oo_OoDynamicA, call2);
PHP_METHOD(Stub_Oo_OoDynamicA, call1);
PHP_METHOD(Stub_Oo_OoDynamicA, execute);
PHP_METHOD(Stub_Oo_OoDynamicA, execute2);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_oodynamica_getnew, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_oodynamica_call2, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_oodynamica_call1, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_oodynamica_execute, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_oodynamica_execute2, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_oo_oodynamica_method_entry) {
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Oo_OoDynamicA, getNew, arginfo_stub_oo_oodynamica_getnew, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
#else
	PHP_ME(Stub_Oo_OoDynamicA, getNew, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Oo_OoDynamicA, call2, arginfo_stub_oo_oodynamica_call2, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
#else
	PHP_ME(Stub_Oo_OoDynamicA, call2, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Oo_OoDynamicA, call1, arginfo_stub_oo_oodynamica_call1, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
#else
	PHP_ME(Stub_Oo_OoDynamicA, call1, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Oo_OoDynamicA, execute, arginfo_stub_oo_oodynamica_execute, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Oo_OoDynamicA, execute, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Oo_OoDynamicA, execute2, arginfo_stub_oo_oodynamica_execute2, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Oo_OoDynamicA, execute2, NULL, ZEND_ACC_PUBLIC)
#endif
	PHP_FE_END
};
