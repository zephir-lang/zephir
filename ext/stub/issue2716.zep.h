
extern zend_class_entry *stub_issue2716_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2716);

PHP_METHOD(Stub_Issue2716, __construct);
PHP_METHOD(Stub_Issue2716, emptyWithString);
PHP_METHOD(Stub_Issue2716, emptyWithArray);
PHP_METHOD(Stub_Issue2716, commentOnlyBody);
PHP_METHOD(Stub_Issue2716, swallowedThrow);
PHP_METHOD(Stub_Issue2716, constructProbe);
PHP_METHOD(Stub_Issue2716, stringProbe);
PHP_METHOD(Stub_Issue2716, arrayProbe);
PHP_METHOD(Stub_Issue2716, commentProbe);
PHP_METHOD(Stub_Issue2716, throwProbe);
PHP_METHOD(Stub_Issue2716, leakProbeControl);
zend_object *zephir_init_properties_Stub_Issue2716(zend_class_entry *class_type);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2716___construct, 0, 0, 0)
ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, options, IS_ARRAY, 0, "[]")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2716_emptywithstring, 0, 1, IS_VOID, 0)

	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2716_emptywitharray, 0, 1, IS_VOID, 0)

	ZEND_ARG_ARRAY_INFO(0, items, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2716_commentonlybody, 0, 0, IS_VOID, 0)

	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, name, IS_STRING, 0, "'x'")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2716_swallowedthrow, 0, 1, IS_VOID, 0)

	ZEND_ARG_ARRAY_INFO(0, items, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2716_constructprobe, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, iterations, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2716_stringprobe, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, iterations, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2716_arrayprobe, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, iterations, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2716_commentprobe, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, iterations, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2716_throwprobe, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, iterations, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2716_leakprobecontrol, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, iterations, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2716_zephir_init_properties_stub_issue2716, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2716_method_entry) {
	PHP_ME(Stub_Issue2716, __construct, arginfo_stub_issue2716___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Stub_Issue2716, emptyWithString, arginfo_stub_issue2716_emptywithstring, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2716, emptyWithArray, arginfo_stub_issue2716_emptywitharray, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2716, commentOnlyBody, arginfo_stub_issue2716_commentonlybody, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2716, swallowedThrow, arginfo_stub_issue2716_swallowedthrow, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2716, constructProbe, arginfo_stub_issue2716_constructprobe, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2716, stringProbe, arginfo_stub_issue2716_stringprobe, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2716, arrayProbe, arginfo_stub_issue2716_arrayprobe, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2716, commentProbe, arginfo_stub_issue2716_commentprobe, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2716, throwProbe, arginfo_stub_issue2716_throwprobe, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2716, leakProbeControl, arginfo_stub_issue2716_leakprobecontrol, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
