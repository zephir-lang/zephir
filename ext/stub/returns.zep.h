
extern zend_class_entry *stub_returns_ce;

ZEPHIR_INIT_CLASS(Stub_Returns);

PHP_METHOD(Stub_Returns, testReturnCast1);
PHP_METHOD(Stub_Returns, testReturnCast2);
PHP_METHOD(Stub_Returns, testReturnCast3);
PHP_METHOD(Stub_Returns, testReturnCast4);
PHP_METHOD(Stub_Returns, returnWithParameter);
PHP_METHOD(Stub_Returns, returnWithoutParameter);
PHP_METHOD(Stub_Returns, returnNullOnString);
PHP_METHOD(Stub_Returns, returnNullOnInt);
PHP_METHOD(Stub_Returns, returnNullOnArray);
PHP_METHOD(Stub_Returns, returnNullOnDouble);
PHP_METHOD(Stub_Returns, returnValidString);
PHP_METHOD(Stub_Returns, returnNullOnNullableString);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_returns_testreturncast1, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_returns_testreturncast2, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_returns_testreturncast3, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_returns_testreturncast4, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_returns_returnwithparameter, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, parameter, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_returns_returnwithoutparameter, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_returns_returnnullonstring, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_returns_returnnullonint, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_returns_returnnullonarray, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_returns_returnnullondouble, 0, 0, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_returns_returnvalidstring, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_returns_returnnullonnullablestring, 0, 0, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_returns_method_entry) {
	PHP_ME(Stub_Returns, testReturnCast1, arginfo_stub_returns_testreturncast1, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Returns, testReturnCast2, arginfo_stub_returns_testreturncast2, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Returns, testReturnCast3, arginfo_stub_returns_testreturncast3, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Returns, testReturnCast4, arginfo_stub_returns_testreturncast4, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Returns, returnWithParameter, arginfo_stub_returns_returnwithparameter, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
PHP_ME(Stub_Returns, returnWithoutParameter, arginfo_stub_returns_returnwithoutparameter, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Returns, returnNullOnString, arginfo_stub_returns_returnnullonstring, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Returns, returnNullOnInt, arginfo_stub_returns_returnnullonint, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Returns, returnNullOnArray, arginfo_stub_returns_returnnullonarray, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Returns, returnNullOnDouble, arginfo_stub_returns_returnnullondouble, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Returns, returnValidString, arginfo_stub_returns_returnvalidstring, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Returns, returnNullOnNullableString, arginfo_stub_returns_returnnullonnullablestring, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
