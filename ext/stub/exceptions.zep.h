
extern zend_class_entry *stub_exceptions_ce;

ZEPHIR_INIT_CLASS(Stub_Exceptions);

PHP_METHOD(Stub_Exceptions, testException1);
PHP_METHOD(Stub_Exceptions, testExceptionStringEscape);
PHP_METHOD(Stub_Exceptions, testException2);
PHP_METHOD(Stub_Exceptions, testException3);
PHP_METHOD(Stub_Exceptions, getException);
PHP_METHOD(Stub_Exceptions, testException4);
PHP_METHOD(Stub_Exceptions, testException5);
PHP_METHOD(Stub_Exceptions, testExceptionLiteral);
PHP_METHOD(Stub_Exceptions, testExceptionSprintf);
PHP_METHOD(Stub_Exceptions, testExceptionConcat);
PHP_METHOD(Stub_Exceptions, testExceptionRethrow);
PHP_METHOD(Stub_Exceptions, testMultiException);
PHP_METHOD(Stub_Exceptions, issue1325);
PHP_METHOD(Stub_Exceptions, doNoopException);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_exceptions_testexception1, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_exceptions_testexceptionstringescape, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_exceptions_testexception2, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_exceptions_testexception3, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_exceptions_getexception, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_exceptions_testexception4, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_exceptions_testexception5, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_exceptions_testexceptionliteral, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, type, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_exceptions_testexceptionsprintf, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_exceptions_testexceptionconcat, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, framework, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, language, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_exceptions_testexceptionrethrow, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_exceptions_testmultiexception, 0, 0, 2)
	ZEND_ARG_INFO(0, returnValue)
	ZEND_ARG_INFO(0, exception)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_exceptions_issue1325, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_exceptions_donoopexception, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_exceptions_method_entry) {
PHP_ME(Stub_Exceptions, testException1, arginfo_stub_exceptions_testexception1, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Exceptions, testExceptionStringEscape, arginfo_stub_exceptions_testexceptionstringescape, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Exceptions, testException2, arginfo_stub_exceptions_testexception2, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Exceptions, testException3, arginfo_stub_exceptions_testexception3, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Exceptions, getException, arginfo_stub_exceptions_getexception, ZEND_ACC_PROTECTED)
PHP_ME(Stub_Exceptions, testException4, arginfo_stub_exceptions_testexception4, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Exceptions, testException5, arginfo_stub_exceptions_testexception5, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Exceptions, testExceptionLiteral, arginfo_stub_exceptions_testexceptionliteral, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Exceptions, testExceptionSprintf, arginfo_stub_exceptions_testexceptionsprintf, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Exceptions, testExceptionConcat, arginfo_stub_exceptions_testexceptionconcat, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Exceptions, testExceptionRethrow, arginfo_stub_exceptions_testexceptionrethrow, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Exceptions, testMultiException, arginfo_stub_exceptions_testmultiexception, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Exceptions, issue1325, arginfo_stub_exceptions_issue1325, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Exceptions, doNoopException, arginfo_stub_exceptions_donoopexception, ZEND_ACC_PRIVATE)
	PHP_FE_END
};
