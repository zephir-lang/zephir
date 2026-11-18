
extern zend_class_entry *stub_issue2839pair_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2839Pair);

PHP_METHOD(Stub_Issue2839Pair, __construct);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2839pair___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO_WITH_DEFAULT_VALUE(0, b, "null")
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2839pair_method_entry) {
	PHP_ME(Stub_Issue2839Pair, __construct, arginfo_stub_issue2839pair___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_FE_END
};
