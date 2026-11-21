
extern zend_class_entry *stub_issue2824renamed_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2824Renamed);

PHP_METHOD(Stub_Issue2824Renamed, pair);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2824renamed_pair, 0, 2, IS_STRING, 0)
	ZEND_ARG_INFO(0, first)
	ZEND_ARG_INFO(0, second)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2824renamed_method_entry) {
	PHP_ME(Stub_Issue2824Renamed, pair, arginfo_stub_issue2824renamed_pair, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
