
extern zend_class_entry *stub_issue2805_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2805);

PHP_METHOD(Stub_Issue2805, hits);
PHP_METHOD(Stub_Issue2805, readMagic);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2805_hits, 0, 1, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2805_readmagic, 0, 2, IS_LONG, 0)
	ZEND_ARG_INFO(0, o)
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2805_zephir_init_static_properties_stub_issue2805, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2805_method_entry) {
	PHP_ME(Stub_Issue2805, hits, arginfo_stub_issue2805_hits, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2805, readMagic, arginfo_stub_issue2805_readmagic, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
