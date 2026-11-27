
extern zend_class_entry *stub_issue2799multiline_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2799Multiline);

PHP_METHOD(Stub_Issue2799Multiline, withDefault);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2799multiline_withdefault, 0, 0, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, x, IS_STRING, 0, "'p1\np2'")
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2799multiline_method_entry) {
	PHP_ME(Stub_Issue2799Multiline, withDefault, arginfo_stub_issue2799multiline_withdefault, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
