
extern zend_class_entry *stub_issue2635_outer_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2635_Outer);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2635_outer_extra, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2635_outer_method_entry) {
	PHP_ABSTRACT_ME(Stub_Issue2635_Outer, extra, arginfo_stub_issue2635_outer_extra)
	PHP_FE_END
};
