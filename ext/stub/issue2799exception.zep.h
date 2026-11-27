
extern zend_class_entry *stub_issue2799exception_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2799Exception);

PHP_METHOD(Stub_Issue2799Exception, getErrors);
PHP_METHOD(Stub_Issue2799Exception, getCodes);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2799exception_geterrors, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2799exception_getcodes, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2799exception_method_entry) {
PHP_ME(Stub_Issue2799Exception, getErrors, arginfo_stub_issue2799exception_geterrors, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Issue2799Exception, getCodes, arginfo_stub_issue2799exception_getcodes, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
