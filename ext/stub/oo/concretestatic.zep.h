
extern zend_class_entry *stub_oo_concretestatic_ce;

ZEPHIR_INIT_CLASS(Stub_Oo_ConcreteStatic);

PHP_METHOD(Stub_Oo_ConcreteStatic, parentFunction);
PHP_METHOD(Stub_Oo_ConcreteStatic, childFunction);
PHP_METHOD(Stub_Oo_ConcreteStatic, simpleStaticIntReturn);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_concretestatic_parentfunction, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_concretestatic_childfunction, 0, 0, 0)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_oo_concretestatic_simplestaticintreturn, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_oo_concretestatic_simplestaticintreturn, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_oo_concretestatic_method_entry) {
	PHP_ME(Stub_Oo_ConcreteStatic, parentFunction, arginfo_stub_oo_concretestatic_parentfunction, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Oo_ConcreteStatic, childFunction, arginfo_stub_oo_concretestatic_childfunction, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Oo_ConcreteStatic, simpleStaticIntReturn, arginfo_stub_oo_concretestatic_simplestaticintreturn, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
