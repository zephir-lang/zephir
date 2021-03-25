
extern zend_class_entry *stub_interfaces_implementint_ce;

ZEPHIR_INIT_CLASS(Stub_Interfaces_ImplementInt);

PHP_METHOD(Stub_Interfaces_ImplementInt, set);
PHP_METHOD(Stub_Interfaces_ImplementInt, get);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_interfaces_implementint_set, 0, 1, IS_VOID, 0)

	ZEND_ARG_TYPE_INFO(0, val, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_interfaces_implementint_get, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_interfaces_implementint_method_entry) {
	PHP_ME(Stub_Interfaces_ImplementInt, set, arginfo_stub_interfaces_implementint_set, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Interfaces_ImplementInt, get, arginfo_stub_interfaces_implementint_get, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
