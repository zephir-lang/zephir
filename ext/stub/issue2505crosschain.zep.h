
extern zend_class_entry *stub_issue2505crosschain_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2505CrossChain);

PHP_METHOD(Stub_Issue2505CrossChain, crossClassChain);
PHP_METHOD(Stub_Issue2505CrossChain, crossClassChainSelf);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2505crosschain_crossclasschain, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_OBJ_INFO(0, instance, Stub\\Issue2505, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2505crosschain_crossclasschainself, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_OBJ_INFO(0, instance, Stub\\Issue2505, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2505crosschain_method_entry) {
	PHP_ME(Stub_Issue2505CrossChain, crossClassChain, arginfo_stub_issue2505crosschain_crossclasschain, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2505CrossChain, crossClassChainSelf, arginfo_stub_issue2505crosschain_crossclasschainself, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
