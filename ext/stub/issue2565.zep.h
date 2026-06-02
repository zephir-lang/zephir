
extern zend_class_entry *stub_issue2565_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2565);

PHP_METHOD(Stub_Issue2565, resolveType);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2565_resolvetype, 0, 1, IS_STRING, 1)
	ZEND_ARG_OBJ_INFO(0, type, ReflectionType, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2565_method_entry) {
	PHP_ME(Stub_Issue2565, resolveType, arginfo_stub_issue2565_resolvetype, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
