
extern zend_class_entry *stub_exception_ce;

ZEPHIR_INIT_CLASS(Stub_Exception);

PHP_METHOD(Stub_Exception, testRuntimePropertyFetch);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_exception_testruntimepropertyfetch, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, message, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_exception_method_entry) {
	PHP_ME(Stub_Exception, testRuntimePropertyFetch, arginfo_stub_exception_testruntimepropertyfetch, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
