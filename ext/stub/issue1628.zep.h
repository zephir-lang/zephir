
extern zend_class_entry *stub_issue1628_ce;

ZEPHIR_INIT_CLASS(Stub_Issue1628);

PHP_METHOD(Stub_Issue1628, mustImplement);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue1628_mustimplement, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue1628_method_entry) {
	PHP_ME(Stub_Issue1628, mustImplement, arginfo_stub_issue1628_mustimplement, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
