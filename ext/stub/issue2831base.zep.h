
extern zend_class_entry *stub_issue2831base_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2831Base);

PHP_METHOD(Stub_Issue2831Base, readOwnTyped);
PHP_METHOD(Stub_Issue2831Base, readOwnTypedInCondition);
PHP_METHOD(Stub_Issue2831Base, readOtherProtected);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2831base_readowntyped, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2831base_readowntypedincondition, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2831base_readotherprotected, 0, 0, 1)
	ZEND_ARG_INFO(0, o)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2831base_method_entry) {
PHP_ME(Stub_Issue2831Base, readOwnTyped, arginfo_stub_issue2831base_readowntyped, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2831Base, readOwnTypedInCondition, arginfo_stub_issue2831base_readowntypedincondition, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2831Base, readOtherProtected, arginfo_stub_issue2831base_readotherprotected, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
