
extern zend_class_entry *stub_issue2391_docblocktypes_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2391_DocblockTypes);

PHP_METHOD(Stub_Issue2391_DocblockTypes, getFloatValue);
PHP_METHOD(Stub_Issue2391_DocblockTypes, setFloatValue);
PHP_METHOD(Stub_Issue2391_DocblockTypes, getIntegerValue);
PHP_METHOD(Stub_Issue2391_DocblockTypes, setIntegerValue);
PHP_METHOD(Stub_Issue2391_DocblockTypes, getBooleanValue);
PHP_METHOD(Stub_Issue2391_DocblockTypes, setBooleanValue);
PHP_METHOD(Stub_Issue2391_DocblockTypes, getNullableInt);
PHP_METHOD(Stub_Issue2391_DocblockTypes, setNullableInt);
PHP_METHOD(Stub_Issue2391_DocblockTypes, getNullableThing);
PHP_METHOD(Stub_Issue2391_DocblockTypes, setNullableThing);
PHP_METHOD(Stub_Issue2391_DocblockTypes, getStrings);
PHP_METHOD(Stub_Issue2391_DocblockTypes, setStrings);
PHP_METHOD(Stub_Issue2391_DocblockTypes, getGeneric);
PHP_METHOD(Stub_Issue2391_DocblockTypes, setGeneric);
PHP_METHOD(Stub_Issue2391_DocblockTypes, getThings);
PHP_METHOD(Stub_Issue2391_DocblockTypes, setThings);
PHP_METHOD(Stub_Issue2391_DocblockTypes, getNumericValue);
PHP_METHOD(Stub_Issue2391_DocblockTypes, setNumericValue);
PHP_METHOD(Stub_Issue2391_DocblockTypes, getScalarValue);
PHP_METHOD(Stub_Issue2391_DocblockTypes, setScalarValue);
PHP_METHOD(Stub_Issue2391_DocblockTypes, getSelfValue);
PHP_METHOD(Stub_Issue2391_DocblockTypes, setSelfValue);
PHP_METHOD(Stub_Issue2391_DocblockTypes, getIterableValue);
PHP_METHOD(Stub_Issue2391_DocblockTypes, setIterableValue);
PHP_METHOD(Stub_Issue2391_DocblockTypes, getVoidValue);
PHP_METHOD(Stub_Issue2391_DocblockTypes, setVoidValue);
PHP_METHOD(Stub_Issue2391_DocblockTypes, getTrueValue);
PHP_METHOD(Stub_Issue2391_DocblockTypes, setTrueValue);
PHP_METHOD(Stub_Issue2391_DocblockTypes, getNullValue);
PHP_METHOD(Stub_Issue2391_DocblockTypes, setNullValue);
PHP_METHOD(Stub_Issue2391_DocblockTypes, getNumericOrNull);
PHP_METHOD(Stub_Issue2391_DocblockTypes, setNumericOrNull);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2391_docblocktypes_getfloatvalue, 0, 0, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2391_docblocktypes_setfloatvalue, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, floatValue, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2391_docblocktypes_getintegervalue, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2391_docblocktypes_setintegervalue, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, integerValue, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2391_docblocktypes_getbooleanvalue, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2391_docblocktypes_setbooleanvalue, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, booleanValue, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2391_docblocktypes_getnullableint, 0, 0, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2391_docblocktypes_setnullableint, 0, 0, 1)
	ZEND_ARG_INFO(0, nullableInt)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_issue2391_docblocktypes_getnullablething, 0, 0, Stub\\Issue2391\\Collaborator, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2391_docblocktypes_setnullablething, 0, 0, 1)
	ZEND_ARG_INFO(0, nullableThing)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2391_docblocktypes_getstrings, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2391_docblocktypes_setstrings, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, strings, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2391_docblocktypes_getgeneric, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2391_docblocktypes_setgeneric, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, generic, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2391_docblocktypes_getthings, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2391_docblocktypes_setthings, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, things, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2391_docblocktypes_getnumericvalue, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2391_docblocktypes_setnumericvalue, 0, 0, 1)
	ZEND_ARG_INFO(0, numericValue)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2391_docblocktypes_getscalarvalue, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2391_docblocktypes_setscalarvalue, 0, 0, 1)
	ZEND_ARG_INFO(0, scalarValue)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2391_docblocktypes_getselfvalue, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2391_docblocktypes_setselfvalue, 0, 0, 1)
	ZEND_ARG_INFO(0, selfValue)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2391_docblocktypes_getiterablevalue, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2391_docblocktypes_setiterablevalue, 0, 0, 1)
	ZEND_ARG_INFO(0, iterableValue)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2391_docblocktypes_getvoidvalue, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2391_docblocktypes_setvoidvalue, 0, 0, 1)
	ZEND_ARG_INFO(0, voidValue)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2391_docblocktypes_gettruevalue, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2391_docblocktypes_settruevalue, 0, 0, 1)
	ZEND_ARG_INFO(0, trueValue)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2391_docblocktypes_getnullvalue, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2391_docblocktypes_setnullvalue, 0, 0, 1)
	ZEND_ARG_INFO(0, nullValue)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2391_docblocktypes_getnumericornull, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2391_docblocktypes_setnumericornull, 0, 0, 1)
	ZEND_ARG_INFO(0, numericOrNull)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2391_docblocktypes_method_entry) {
	PHP_ME(Stub_Issue2391_DocblockTypes, getFloatValue, arginfo_stub_issue2391_docblocktypes_getfloatvalue, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2391_DocblockTypes, setFloatValue, arginfo_stub_issue2391_docblocktypes_setfloatvalue, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2391_DocblockTypes, getIntegerValue, arginfo_stub_issue2391_docblocktypes_getintegervalue, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2391_DocblockTypes, setIntegerValue, arginfo_stub_issue2391_docblocktypes_setintegervalue, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2391_DocblockTypes, getBooleanValue, arginfo_stub_issue2391_docblocktypes_getbooleanvalue, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2391_DocblockTypes, setBooleanValue, arginfo_stub_issue2391_docblocktypes_setbooleanvalue, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2391_DocblockTypes, getNullableInt, arginfo_stub_issue2391_docblocktypes_getnullableint, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2391_DocblockTypes, setNullableInt, arginfo_stub_issue2391_docblocktypes_setnullableint, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2391_DocblockTypes, getNullableThing, arginfo_stub_issue2391_docblocktypes_getnullablething, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2391_DocblockTypes, setNullableThing, arginfo_stub_issue2391_docblocktypes_setnullablething, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2391_DocblockTypes, getStrings, arginfo_stub_issue2391_docblocktypes_getstrings, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2391_DocblockTypes, setStrings, arginfo_stub_issue2391_docblocktypes_setstrings, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2391_DocblockTypes, getGeneric, arginfo_stub_issue2391_docblocktypes_getgeneric, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2391_DocblockTypes, setGeneric, arginfo_stub_issue2391_docblocktypes_setgeneric, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2391_DocblockTypes, getThings, arginfo_stub_issue2391_docblocktypes_getthings, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2391_DocblockTypes, setThings, arginfo_stub_issue2391_docblocktypes_setthings, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Issue2391_DocblockTypes, getNumericValue, arginfo_stub_issue2391_docblocktypes_getnumericvalue, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2391_DocblockTypes, setNumericValue, arginfo_stub_issue2391_docblocktypes_setnumericvalue, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Issue2391_DocblockTypes, getScalarValue, arginfo_stub_issue2391_docblocktypes_getscalarvalue, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2391_DocblockTypes, setScalarValue, arginfo_stub_issue2391_docblocktypes_setscalarvalue, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Issue2391_DocblockTypes, getSelfValue, arginfo_stub_issue2391_docblocktypes_getselfvalue, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2391_DocblockTypes, setSelfValue, arginfo_stub_issue2391_docblocktypes_setselfvalue, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Issue2391_DocblockTypes, getIterableValue, arginfo_stub_issue2391_docblocktypes_getiterablevalue, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2391_DocblockTypes, setIterableValue, arginfo_stub_issue2391_docblocktypes_setiterablevalue, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Issue2391_DocblockTypes, getVoidValue, arginfo_stub_issue2391_docblocktypes_getvoidvalue, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2391_DocblockTypes, setVoidValue, arginfo_stub_issue2391_docblocktypes_setvoidvalue, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Issue2391_DocblockTypes, getTrueValue, arginfo_stub_issue2391_docblocktypes_gettruevalue, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2391_DocblockTypes, setTrueValue, arginfo_stub_issue2391_docblocktypes_settruevalue, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Issue2391_DocblockTypes, getNullValue, arginfo_stub_issue2391_docblocktypes_getnullvalue, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2391_DocblockTypes, setNullValue, arginfo_stub_issue2391_docblocktypes_setnullvalue, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Issue2391_DocblockTypes, getNumericOrNull, arginfo_stub_issue2391_docblocktypes_getnumericornull, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2391_DocblockTypes, setNumericOrNull, arginfo_stub_issue2391_docblocktypes_setnumericornull, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
