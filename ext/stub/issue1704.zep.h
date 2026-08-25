
extern zend_class_entry *stub_issue1704_ce;

ZEPHIR_INIT_CLASS(Stub_Issue1704);

PHP_METHOD(Stub_Issue1704, runmode);
PHP_METHOD(Stub_Issue1704, padded);
PHP_METHOD(Stub_Issue1704, breakAfterReturnInLoop);
PHP_METHOD(Stub_Issue1704, continueAfterReturnInLoop);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue1704_runmode, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, value, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue1704_padded, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, value, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue1704_breakafterreturninloop, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, limit, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue1704_continueafterreturninloop, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, limit, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue1704_method_entry) {
	PHP_ME(Stub_Issue1704, runmode, arginfo_stub_issue1704_runmode, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue1704, padded, arginfo_stub_issue1704_padded, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue1704, breakAfterReturnInLoop, arginfo_stub_issue1704_breakafterreturninloop, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue1704, continueAfterReturnInLoop, arginfo_stub_issue1704_continueafterreturninloop, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
