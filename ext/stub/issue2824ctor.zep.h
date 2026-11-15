
extern zend_class_entry *stub_issue2824ctor_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2824Ctor);

PHP_METHOD(Stub_Issue2824Ctor, __construct);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2824ctor___construct, 0, 0, 0)
	ZEND_ARG_INFO_WITH_DEFAULT_VALUE(0, a, "'x'")
	ZEND_ARG_INFO_WITH_DEFAULT_VALUE(0, b, "'y'")
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2824ctor_method_entry) {
	PHP_ME(Stub_Issue2824Ctor, __construct, arginfo_stub_issue2824ctor___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_FE_END
};
