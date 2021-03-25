
extern zend_class_entry *stub_oo_abstractstatic_ce;

ZEPHIR_INIT_CLASS(Stub_Oo_AbstractStatic);

PHP_METHOD(Stub_Oo_AbstractStatic, getCalledClass);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_oo_abstractstatic_getcalledclass, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_oo_abstractstatic_method_entry) {
	PHP_ME(Stub_Oo_AbstractStatic, getCalledClass, arginfo_stub_oo_abstractstatic_getcalledclass, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
