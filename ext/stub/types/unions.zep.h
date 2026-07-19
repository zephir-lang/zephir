
extern zend_class_entry *stub_types_unions_ce;

ZEPHIR_INIT_CLASS(Stub_Types_Unions);

PHP_METHOD(Stub_Types_Unions, kind);
PHP_METHOD(Stub_Types_Unions, pick);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_types_unions_kind, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_MASK(0, value, MAY_BE_LONG|MAY_BE_DOUBLE, NULL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_types_unions_pick, 0, 1, IS_STRING, 0)
	ZEND_ARG_OBJ_TYPE_MASK(0, value, Stub\\Types\\MayBe, MAY_BE_STRING, NULL)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_types_unions_method_entry) {
	PHP_ME(Stub_Types_Unions, kind, arginfo_stub_types_unions_kind, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Types_Unions, pick, arginfo_stub_types_unions_pick, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
