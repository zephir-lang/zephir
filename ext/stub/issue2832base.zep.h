
extern zend_class_entry *stub_issue2832base_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2832Base);

PHP_METHOD(Stub_Issue2832Base, fetchName);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2832base_fetchname, 0, 2, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, o)
	ZEND_ARG_INFO(0, n)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2832base_method_entry) {
	PHP_ME(Stub_Issue2832Base, fetchName, arginfo_stub_issue2832base_fetchname, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
