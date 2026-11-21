
extern zend_class_entry *stub_issue2831child_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2831Child);

PHP_METHOD(Stub_Issue2831Child, readParentProtected);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2831child_readparentprotected, 0, 0, 1)
	ZEND_ARG_INFO(0, o)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2831child_method_entry) {
	PHP_ME(Stub_Issue2831Child, readParentProtected, arginfo_stub_issue2831child_readparentprotected, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
