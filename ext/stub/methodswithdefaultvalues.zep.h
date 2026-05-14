
extern zend_class_entry *stub_methodswithdefaultvalues_ce;

ZEPHIR_INIT_CLASS(Stub_MethodsWithDefaultValues);

PHP_METHOD(Stub_MethodsWithDefaultValues, testBool);
PHP_METHOD(Stub_MethodsWithDefaultValues, testNullableBool);
PHP_METHOD(Stub_MethodsWithDefaultValues, testString);
PHP_METHOD(Stub_MethodsWithDefaultValues, testNullableString);
PHP_METHOD(Stub_MethodsWithDefaultValues, testInt);
PHP_METHOD(Stub_MethodsWithDefaultValues, testNullableInt);
PHP_METHOD(Stub_MethodsWithDefaultValues, testDouble);
PHP_METHOD(Stub_MethodsWithDefaultValues, testNullableDouble);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_methodswithdefaultvalues_testbool, 0, 0, IS_VOID, 0)

	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, param, _IS_BOOL, 0, "true")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_methodswithdefaultvalues_testnullablebool, 0, 0, IS_VOID, 0)

	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, param, _IS_BOOL, 1, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_methodswithdefaultvalues_teststring, 0, 0, IS_VOID, 0)

	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, param, IS_STRING, 0, "'va\\'lue'")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_methodswithdefaultvalues_testnullablestring, 0, 0, IS_VOID, 0)

	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, param, IS_STRING, 1, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_methodswithdefaultvalues_testint, 0, 0, IS_VOID, 0)

	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, param, IS_LONG, 0, "1337")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_methodswithdefaultvalues_testnullableint, 0, 0, IS_VOID, 0)

	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, param, IS_LONG, 1, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_methodswithdefaultvalues_testdouble, 0, 0, IS_VOID, 0)

	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, param, IS_DOUBLE, 0, "13.37")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_methodswithdefaultvalues_testnullabledouble, 0, 0, IS_VOID, 0)

	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, param, IS_DOUBLE, 1, "null")
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_methodswithdefaultvalues_method_entry) {
	PHP_ME(Stub_MethodsWithDefaultValues, testBool, arginfo_stub_methodswithdefaultvalues_testbool, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_MethodsWithDefaultValues, testNullableBool, arginfo_stub_methodswithdefaultvalues_testnullablebool, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_MethodsWithDefaultValues, testString, arginfo_stub_methodswithdefaultvalues_teststring, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_MethodsWithDefaultValues, testNullableString, arginfo_stub_methodswithdefaultvalues_testnullablestring, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_MethodsWithDefaultValues, testInt, arginfo_stub_methodswithdefaultvalues_testint, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_MethodsWithDefaultValues, testNullableInt, arginfo_stub_methodswithdefaultvalues_testnullableint, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_MethodsWithDefaultValues, testDouble, arginfo_stub_methodswithdefaultvalues_testdouble, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_MethodsWithDefaultValues, testNullableDouble, arginfo_stub_methodswithdefaultvalues_testnullabledouble, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
