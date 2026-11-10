
extern zend_class_entry *stub_issue2805request_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2805Request);

PHP_METHOD(Stub_Issue2805Request, bump);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2805request_bump, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2805request_method_entry) {
	PHP_ME(Stub_Issue2805Request, bump, arginfo_stub_issue2805request_bump, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
