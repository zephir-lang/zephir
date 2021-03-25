
extern zend_class_entry *stub_oo_ooconstructparams_ce;

ZEPHIR_INIT_CLASS(Stub_Oo_OoConstructParams);

PHP_METHOD(Stub_Oo_OoConstructParams, __construct);
PHP_METHOD(Stub_Oo_OoConstructParams, getA);
PHP_METHOD(Stub_Oo_OoConstructParams, getB);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_ooconstructparams___construct, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_ooconstructparams_geta, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_ooconstructparams_getb, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_oo_ooconstructparams_method_entry) {
	PHP_ME(Stub_Oo_OoConstructParams, __construct, arginfo_stub_oo_ooconstructparams___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Oo_OoConstructParams, getA, arginfo_stub_oo_ooconstructparams_geta, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Oo_OoConstructParams, getA, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Oo_OoConstructParams, getB, arginfo_stub_oo_ooconstructparams_getb, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Oo_OoConstructParams, getB, NULL, ZEND_ACC_PUBLIC)
#endif
	PHP_FE_END
};
