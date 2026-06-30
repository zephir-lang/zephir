
extern zend_class_entry *stub_issue1628abstract_ce;

ZEPHIR_INIT_CLASS(Stub_Issue1628Abstract);

PHP_METHOD(Stub_Issue1628Abstract, concrete);
PHP_METHOD(Stub_Issue1628Abstract, mustImplement);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue1628abstract_concrete, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue1628abstract_mustimplement, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue1628abstract_method_entry) {
	PHP_ME(Stub_Issue1628Abstract, concrete, arginfo_stub_issue1628abstract_concrete, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue1628Abstract, mustImplement, arginfo_stub_issue1628abstract_mustimplement, ZEND_ACC_ABSTRACT|ZEND_ACC_PUBLIC)
	PHP_FE_END
};
