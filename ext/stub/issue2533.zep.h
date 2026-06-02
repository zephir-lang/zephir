
extern zend_class_entry *stub_issue2533_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2533);

PHP_METHOD(Stub_Issue2533, bar);
PHP_METHOD(Stub_Issue2533, map);
PHP_METHOD(Stub_Issue2533, nested);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2533_bar, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2533_map, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2533_nested, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2533_method_entry) {
	PHP_ME(Stub_Issue2533, bar, arginfo_stub_issue2533_bar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2533, map, arginfo_stub_issue2533_map, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2533, nested, arginfo_stub_issue2533_nested, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
