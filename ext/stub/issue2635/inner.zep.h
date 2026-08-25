
extern zend_class_entry *stub_issue2635_inner_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2635_Inner);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2635_inner_go, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2635_inner_method_entry) {
	PHP_ABSTRACT_ME(Stub_Issue2635_Inner, go, arginfo_stub_issue2635_inner_go)
	PHP_FE_END
};
