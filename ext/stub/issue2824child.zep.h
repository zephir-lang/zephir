
extern zend_class_entry *stub_issue2824child_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2824Child);

PHP_METHOD(Stub_Issue2824Child, pair);
PHP_METHOD(Stub_Issue2824Child, parentPair);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2824child_pair, 0, 2, IS_STRING, 0)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2824child_parentpair, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2824child_method_entry) {
	PHP_ME(Stub_Issue2824Child, pair, arginfo_stub_issue2824child_pair, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2824Child, parentPair, arginfo_stub_issue2824child_parentpair, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
