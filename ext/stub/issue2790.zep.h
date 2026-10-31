
extern zend_class_entry *stub_issue2790_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2790);

PHP_METHOD(Stub_Issue2790, callWho);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2790_callwho, 0, 1, IS_STRING, 0)
	ZEND_ARG_INFO(0, o)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2790_method_entry) {
	PHP_ME(Stub_Issue2790, callWho, arginfo_stub_issue2790_callwho, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
