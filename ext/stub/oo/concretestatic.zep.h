
extern zend_class_entry *stub_oo_concretestatic_ce;

ZEPHIR_INIT_CLASS(Stub_Oo_ConcreteStatic);

PHP_METHOD(Stub_Oo_ConcreteStatic, parentFunction);
PHP_METHOD(Stub_Oo_ConcreteStatic, childFunction);
PHP_METHOD(Stub_Oo_ConcreteStatic, callParentFunction);
PHP_METHOD(Stub_Oo_ConcreteStatic, simpleStaticIntReturn);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_concretestatic_parentfunction, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_concretestatic_childfunction, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_oo_concretestatic_callparentfunction, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_oo_concretestatic_simplestaticintreturn, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_oo_concretestatic_method_entry) {
PHP_ME(Stub_Oo_ConcreteStatic, parentFunction, arginfo_stub_oo_concretestatic_parentfunction, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
PHP_ME(Stub_Oo_ConcreteStatic, childFunction, arginfo_stub_oo_concretestatic_childfunction, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Oo_ConcreteStatic, callParentFunction, arginfo_stub_oo_concretestatic_callparentfunction, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Oo_ConcreteStatic, simpleStaticIntReturn, arginfo_stub_oo_concretestatic_simplestaticintreturn, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
