
extern zend_class_entry *stub_issue2739_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2739);

PHP_METHOD(Stub_Issue2739, now);
PHP_METHOD(Stub_Issue2739, notBefore);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2739_now, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2739_notbefore, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, timestamp, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2739_method_entry) {
	PHP_ME(Stub_Issue2739, now, arginfo_stub_issue2739_now, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2739, notBefore, arginfo_stub_issue2739_notbefore, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
