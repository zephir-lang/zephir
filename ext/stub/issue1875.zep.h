
extern zend_class_entry *stub_issue1875_ce;

ZEPHIR_INIT_CLASS(Stub_Issue1875);

PHP_METHOD(Stub_Issue1875, conditionalThenRead);
PHP_METHOD(Stub_Issue1875, reassignInBranches);
PHP_METHOD(Stub_Issue1875, neverAssigned);
PHP_METHOD(Stub_Issue1875, forwardedToNewInstance);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue1875_conditionalthenread, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, options, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue1875_reassigninbranches, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, options, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue1875_neverassigned, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue1875_forwardedtonewinstance, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, options, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue1875_method_entry) {
	PHP_ME(Stub_Issue1875, conditionalThenRead, arginfo_stub_issue1875_conditionalthenread, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue1875, reassignInBranches, arginfo_stub_issue1875_reassigninbranches, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Issue1875, neverAssigned, arginfo_stub_issue1875_neverassigned, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue1875, forwardedToNewInstance, arginfo_stub_issue1875_forwardedtonewinstance, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
