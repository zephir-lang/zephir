
extern zend_class_entry *stub_issue2635_base_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2635_Base);

PHP_METHOD(Stub_Issue2635_Base, run);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2635_base_run, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2635_base_method_entry) {
	PHP_ME(Stub_Issue2635_Base, run, arginfo_stub_issue2635_base_run, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
