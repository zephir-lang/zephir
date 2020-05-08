
extern zend_class_entry *stub_oo_scopes_hasprivatemethod_ce;

ZEPHIR_INIT_CLASS(Stub_Oo_Scopes_HasPrivateMethod);

PHP_METHOD(Stub_Oo_Scopes_HasPrivateMethod, callPrivateMethod);
PHP_METHOD(Stub_Oo_Scopes_HasPrivateMethod, isPrivate);

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_oo_scopes_hasprivatemethod_callprivatemethod, 0, 0, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_oo_scopes_hasprivatemethod_callprivatemethod, 0, 0, IS_STRING, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_oo_scopes_hasprivatemethod_isprivate, 0, 0, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_oo_scopes_hasprivatemethod_isprivate, 0, 0, IS_STRING, NULL, 0)
#endif
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_oo_scopes_hasprivatemethod_method_entry) {
	PHP_ME(Stub_Oo_Scopes_HasPrivateMethod, callPrivateMethod, arginfo_stub_oo_scopes_hasprivatemethod_callprivatemethod, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_Scopes_HasPrivateMethod, isPrivate, arginfo_stub_oo_scopes_hasprivatemethod_isprivate, ZEND_ACC_PRIVATE)
	PHP_FE_END
};
