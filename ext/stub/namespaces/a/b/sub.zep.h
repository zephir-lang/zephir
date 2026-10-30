
extern zend_class_entry *stub_namespaces_a_b_sub_ce;

ZEPHIR_INIT_CLASS(Stub_Namespaces_A_B_Sub);

PHP_METHOD(Stub_Namespaces_A_B_Sub, issue2686ClosureNew);
PHP_METHOD(Stub_Namespaces_A_B_Sub, issue2686ArrowInstanceOf);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_namespaces_a_b_sub_issue2686closurenew, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_namespaces_a_b_sub_issue2686arrowinstanceof, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_namespaces_a_b_sub_method_entry) {
PHP_ME(Stub_Namespaces_A_B_Sub, issue2686ClosureNew, arginfo_stub_namespaces_a_b_sub_issue2686closurenew, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Namespaces_A_B_Sub, issue2686ArrowInstanceOf, arginfo_stub_namespaces_a_b_sub_issue2686arrowinstanceof, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
