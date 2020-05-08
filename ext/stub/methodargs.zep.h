
extern zend_class_entry *stub_methodargs_ce;

ZEPHIR_INIT_CLASS(Stub_MethodArgs);

PHP_METHOD(Stub_MethodArgs, setCallable);
PHP_METHOD(Stub_MethodArgs, setObject);
PHP_METHOD(Stub_MethodArgs, setCallableStrict);
PHP_METHOD(Stub_MethodArgs, setObjectStrict);
PHP_METHOD(Stub_MethodArgs, setResourceStrict);
PHP_METHOD(Stub_MethodArgs, methodOptionalValueWithDefaultStaticConstantAccess);
PHP_METHOD(Stub_MethodArgs, methodOptionalStringValueWithDefaultStaticConstantAccess);
PHP_METHOD(Stub_MethodArgs, methodOptionalDoubleValueWithDefaultStaticConstantAccess);
PHP_METHOD(Stub_MethodArgs, methodOptionalBoolValueWithDefaultStaticConstantAccess);
PHP_METHOD(Stub_MethodArgs, methodOptionalIntValueWithDefaultStaticConstantAccess);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_methodargs_setcallable, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_methodargs_setobject, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_methodargs_setcallablestrict, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_methodargs_setobjectstrict, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_methodargs_setresourcestrict, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_methodargs_methodoptionalvaluewithdefaultstaticconstantaccess, 0, 0, 0)
	ZEND_ARG_INFO(0, method)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_methodargs_methodoptionalstringvaluewithdefaultstaticconstantaccess, 0, 0, 0)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, parameter, IS_STRING, 0)
#else
	ZEND_ARG_INFO(0, parameter)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_methodargs_methodoptionaldoublevaluewithdefaultstaticconstantaccess, 0, 0, 0)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, parameter, IS_DOUBLE, 0)
#else
	ZEND_ARG_INFO(0, parameter)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_methodargs_methodoptionalboolvaluewithdefaultstaticconstantaccess, 0, 0, 0)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, parameter, _IS_BOOL, 0)
#else
	ZEND_ARG_INFO(0, parameter)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_methodargs_methodoptionalintvaluewithdefaultstaticconstantaccess, 0, 0, 0)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, parameter, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, parameter)
#endif
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_methodargs_method_entry) {
	PHP_ME(Stub_MethodArgs, setCallable, arginfo_stub_methodargs_setcallable, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_MethodArgs, setObject, arginfo_stub_methodargs_setobject, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_MethodArgs, setCallableStrict, arginfo_stub_methodargs_setcallablestrict, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_MethodArgs, setObjectStrict, arginfo_stub_methodargs_setobjectstrict, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_MethodArgs, setResourceStrict, arginfo_stub_methodargs_setresourcestrict, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_MethodArgs, methodOptionalValueWithDefaultStaticConstantAccess, arginfo_stub_methodargs_methodoptionalvaluewithdefaultstaticconstantaccess, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_MethodArgs, methodOptionalStringValueWithDefaultStaticConstantAccess, arginfo_stub_methodargs_methodoptionalstringvaluewithdefaultstaticconstantaccess, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_MethodArgs, methodOptionalDoubleValueWithDefaultStaticConstantAccess, arginfo_stub_methodargs_methodoptionaldoublevaluewithdefaultstaticconstantaccess, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_MethodArgs, methodOptionalBoolValueWithDefaultStaticConstantAccess, arginfo_stub_methodargs_methodoptionalboolvaluewithdefaultstaticconstantaccess, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_MethodArgs, methodOptionalIntValueWithDefaultStaticConstantAccess, arginfo_stub_methodargs_methodoptionalintvaluewithdefaultstaticconstantaccess, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
