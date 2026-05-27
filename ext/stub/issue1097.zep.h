
extern zend_class_entry *stub_issue1097_ce;

ZEPHIR_INIT_CLASS(Stub_Issue1097);

PHP_METHOD(Stub_Issue1097, testStringOrArrayOfString);
PHP_METHOD(Stub_Issue1097, testWithElse);
PHP_METHOD(Stub_Issue1097, testMultipleElseif);
PHP_METHOD(Stub_Issue1097, testSimpleElseif);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue1097_teststringorarrayofstring, 0, 1, IS_STRING, 0)
	ZEND_ARG_INFO(0, myvar)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue1097_testwithelse, 0, 1, IS_STRING, 0)
	ZEND_ARG_INFO(0, myvar)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue1097_testmultipleelseif, 0, 1, IS_STRING, 0)
	ZEND_ARG_INFO(0, myvar)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue1097_testsimpleelseif, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, num, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue1097_method_entry) {
	PHP_ME(Stub_Issue1097, testStringOrArrayOfString, arginfo_stub_issue1097_teststringorarrayofstring, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue1097, testWithElse, arginfo_stub_issue1097_testwithelse, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue1097, testMultipleElseif, arginfo_stub_issue1097_testmultipleelseif, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue1097, testSimpleElseif, arginfo_stub_issue1097_testsimpleelseif, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
