
extern zend_class_entry *stub_requires_external3_ce;

ZEPHIR_INIT_CLASS(Stub_Requires_External3);

PHP_METHOD(Stub_Requires_External3, req);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_requires_external3_req, 0, 2, IS_VOID, 0)

	ZEND_ARG_INFO(0, path)
	ZEND_ARG_INFO(0, requires)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_requires_external3_method_entry) {
	PHP_ME(Stub_Requires_External3, req, arginfo_stub_requires_external3_req, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
