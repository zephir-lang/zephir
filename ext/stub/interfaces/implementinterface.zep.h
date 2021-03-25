
extern zend_class_entry *stub_interfaces_implementinterface_ce;

ZEPHIR_INIT_CLASS(Stub_Interfaces_ImplementInterface);

PHP_METHOD(Stub_Interfaces_ImplementInterface, get);
PHP_METHOD(Stub_Interfaces_ImplementInterface, getVoid);
PHP_METHOD(Stub_Interfaces_ImplementInterface, getObj);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_interfaces_implementinterface_get, 0, 1, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, obj, Stub\\Interfaces\\InterfaceInt, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_interfaces_implementinterface_getvoid, 0, 1, IS_VOID, 0)

	ZEND_ARG_OBJ_INFO(0, obj, Stub\\Interfaces\\InterfaceInt, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_interfaces_implementinterface_getobj, 0, 0, Stub\\Interfaces\\InterfaceInt, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_interfaces_implementinterface_method_entry) {
	PHP_ME(Stub_Interfaces_ImplementInterface, get, arginfo_stub_interfaces_implementinterface_get, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Interfaces_ImplementInterface, getVoid, arginfo_stub_interfaces_implementinterface_getvoid, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Interfaces_ImplementInterface, getObj, arginfo_stub_interfaces_implementinterface_getobj, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
