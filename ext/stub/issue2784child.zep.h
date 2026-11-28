
extern zend_class_entry *stub_issue2784child_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2784Child);

PHP_METHOD(Stub_Issue2784Child, unsetParentProtected);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2784child_unsetparentprotected, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, o)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2784child_method_entry) {
	PHP_ME(Stub_Issue2784Child, unsetParentProtected, arginfo_stub_issue2784child_unsetparentprotected, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
