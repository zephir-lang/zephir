
extern zend_class_entry *stub_issue1706_ce;

ZEPHIR_INIT_CLASS(Stub_Issue1706);

PHP_METHOD(Stub_Issue1706, testSwitch);
PHP_METHOD(Stub_Issue1706, switchDefaultLast);
PHP_METHOD(Stub_Issue1706, switchWithThrow);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue1706_testswitch, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue1706_switchdefaultlast, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue1706_switchwiththrow, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue1706_method_entry) {
	PHP_ME(Stub_Issue1706, testSwitch, arginfo_stub_issue1706_testswitch, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue1706, switchDefaultLast, arginfo_stub_issue1706_switchdefaultlast, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue1706, switchWithThrow, arginfo_stub_issue1706_switchwiththrow, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
