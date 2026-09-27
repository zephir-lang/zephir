
extern zend_class_entry *stub_issue2738_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2738);

PHP_METHOD(Stub_Issue2738, withDefault);
PHP_METHOD(Stub_Issue2738, returned);
PHP_METHOD(Stub_Issue2738, combined);
PHP_METHOD(Stub_Issue2738, described);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2738_withdefault, 0, 0, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, flags, IS_LONG, 0, "\\Attribute::IS_REPEATABLE")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2738_returned, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2738_combined, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2738_described, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2738_method_entry) {
	PHP_ME(Stub_Issue2738, withDefault, arginfo_stub_issue2738_withdefault, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2738, returned, arginfo_stub_issue2738_returned, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2738, combined, arginfo_stub_issue2738_combined, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2738, described, arginfo_stub_issue2738_described, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
