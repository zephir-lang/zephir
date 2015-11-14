
extern zend_class_entry *test_exceptions_ce;

ZEPHIR_INIT_CLASS(Test_Exceptions);

PHP_METHOD(Test_Exceptions, testException1);
PHP_METHOD(Test_Exceptions, testExceptionStringEscape);
PHP_METHOD(Test_Exceptions, testException2);
PHP_METHOD(Test_Exceptions, testException3);
PHP_METHOD(Test_Exceptions, getException);
PHP_METHOD(Test_Exceptions, testException4);
PHP_METHOD(Test_Exceptions, testException5);
PHP_METHOD(Test_Exceptions, testExceptionLiteral);
PHP_METHOD(Test_Exceptions, testExceptionSprintf);
PHP_METHOD(Test_Exceptions, testExceptionConcat);
PHP_METHOD(Test_Exceptions, testExceptionRethrow);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_exceptions_testexceptionliteral, 0, 0, 1)
	ZEND_ARG_INFO(0, type)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_exceptions_testexceptionsprintf, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_exceptions_testexceptionconcat, 0, 0, 2)
	ZEND_ARG_INFO(0, framework)
	ZEND_ARG_INFO(0, language)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_exceptions_method_entry) {
	PHP_ME(Test_Exceptions, testException1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Exceptions, testExceptionStringEscape, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Exceptions, testException2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Exceptions, testException3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Exceptions, getException, NULL, ZEND_ACC_PROTECTED)
	PHP_ME(Test_Exceptions, testException4, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Exceptions, testException5, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Exceptions, testExceptionLiteral, arginfo_test_exceptions_testexceptionliteral, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Exceptions, testExceptionSprintf, arginfo_test_exceptions_testexceptionsprintf, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Exceptions, testExceptionConcat, arginfo_test_exceptions_testexceptionconcat, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Exceptions, testExceptionRethrow, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
