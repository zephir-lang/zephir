
extern zend_class_entry *stub_issue2691_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2691);

PHP_METHOD(Stub_Issue2691, __construct);
PHP_METHOD(Stub_Issue2691, pushAndReturn);
PHP_METHOD(Stub_Issue2691, pushIntoDynamic);
PHP_METHOD(Stub_Issue2691, shiftDynamic);
PHP_METHOD(Stub_Issue2691, pushIntoBracedLiteral);
PHP_METHOD(Stub_Issue2691, pushIntoReadonly);
PHP_METHOD(Stub_Issue2691, getReadonlyRows);
PHP_METHOD(Stub_Issue2691, pushThenCopy);
PHP_METHOD(Stub_Issue2691, pushStatic);
PHP_METHOD(Stub_Issue2691, resetStatic);
PHP_METHOD(Stub_Issue2691, pushIntoLocalShared);
PHP_METHOD(Stub_Issue2691, shiftRows);
PHP_METHOD(Stub_Issue2691, retypeValue);
PHP_METHOD(Stub_Issue2691, pushMissingKey);
PHP_METHOD(Stub_Issue2691, pushIntoOverloaded);
PHP_METHOD(Stub_Issue2691, setRows);
PHP_METHOD(Stub_Issue2691, getRows);
PHP_METHOD(Stub_Issue2691, setValue);
PHP_METHOD(Stub_Issue2691, getValue);
PHP_METHOD(Stub_Issue2691, getPrefixes);
PHP_METHOD(Stub_Issue2691, writeLeakProbe);
PHP_METHOD(Stub_Issue2691, leakProbeControl);
zend_object *zephir_init_properties_Stub_Issue2691(zend_class_entry *class_type);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2691___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2691_pushandreturn, 0, 2, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, value, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2691_pushintodynamic, 0, 3, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, value, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2691_shiftdynamic, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2691_pushintobracedliteral, 0, 2, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, value, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2691_pushintoreadonly, 0, 2, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, value, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2691_getreadonlyrows, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2691_pushthencopy, 0, 2, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, value, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2691_pushstatic, 0, 2, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, value, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2691_resetstatic, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2691_pushintolocalshared, 0, 2, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, value, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2691_shiftrows, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2691_retypevalue, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, type, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2691_pushmissingkey, 0, 2, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, value, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2691_pushintooverloaded, 0, 3, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, holder)
	ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, value, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2691_setrows, 0, 1, IS_VOID, 0)

	ZEND_ARG_INFO(0, rows)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2691_getrows, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2691_setvalue, 0, 1, IS_VOID, 0)

	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2691_getvalue, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2691_getprefixes, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2691_writeleakprobe, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, iterations, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2691_leakprobecontrol, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, iterations, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2691_zephir_init_properties_stub_issue2691, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2691_zephir_init_static_properties_stub_issue2691, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2691_method_entry) {
PHP_ME(Stub_Issue2691, __construct, arginfo_stub_issue2691___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Stub_Issue2691, pushAndReturn, arginfo_stub_issue2691_pushandreturn, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2691, pushIntoDynamic, arginfo_stub_issue2691_pushintodynamic, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2691, shiftDynamic, arginfo_stub_issue2691_shiftdynamic, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2691, pushIntoBracedLiteral, arginfo_stub_issue2691_pushintobracedliteral, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2691, pushIntoReadonly, arginfo_stub_issue2691_pushintoreadonly, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2691, getReadonlyRows, arginfo_stub_issue2691_getreadonlyrows, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2691, pushThenCopy, arginfo_stub_issue2691_pushthencopy, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2691, pushStatic, arginfo_stub_issue2691_pushstatic, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2691, resetStatic, arginfo_stub_issue2691_resetstatic, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2691, pushIntoLocalShared, arginfo_stub_issue2691_pushintolocalshared, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Issue2691, shiftRows, arginfo_stub_issue2691_shiftrows, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2691, retypeValue, arginfo_stub_issue2691_retypevalue, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2691, pushMissingKey, arginfo_stub_issue2691_pushmissingkey, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2691, pushIntoOverloaded, arginfo_stub_issue2691_pushintooverloaded, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2691, setRows, arginfo_stub_issue2691_setrows, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Issue2691, getRows, arginfo_stub_issue2691_getrows, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2691, setValue, arginfo_stub_issue2691_setvalue, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Issue2691, getValue, arginfo_stub_issue2691_getvalue, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2691, getPrefixes, arginfo_stub_issue2691_getprefixes, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2691, writeLeakProbe, arginfo_stub_issue2691_writeleakprobe, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2691, leakProbeControl, arginfo_stub_issue2691_leakprobecontrol, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
