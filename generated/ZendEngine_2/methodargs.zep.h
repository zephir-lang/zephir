
extern zend_class_entry *test_methodargs_ce;

ZEPHIR_INIT_CLASS(Test_MethodArgs);

PHP_METHOD(Test_MethodArgs, setCallable);
PHP_METHOD(Test_MethodArgs, setObject);
PHP_METHOD(Test_MethodArgs, setCallableStrict);
PHP_METHOD(Test_MethodArgs, setObjectStrict);
PHP_METHOD(Test_MethodArgs, setResourceStrict);
PHP_METHOD(Test_MethodArgs, methodOptionalValueWithDefaultStaticConstantAccess);
PHP_METHOD(Test_MethodArgs, methodOptionalStringValueWithDefaultStaticConstantAccess);
PHP_METHOD(Test_MethodArgs, methodOptionalDoubleValueWithDefaultStaticConstantAccess);
PHP_METHOD(Test_MethodArgs, methodOptionalBoolValueWithDefaultStaticConstantAccess);
PHP_METHOD(Test_MethodArgs, methodOptionalIntValueWithDefaultStaticConstantAccess);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_methodargs_setcallable, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_methodargs_setobject, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_methodargs_setcallablestrict, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_methodargs_setobjectstrict, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_methodargs_setresourcestrict, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_methodargs_methodoptionalvaluewithdefaultstaticconstantaccess, 0, 0, 0)
	ZEND_ARG_INFO(0, method)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_methodargs_methodoptionalstringvaluewithdefaultstaticconstantaccess, 0, 0, 0)
	ZEND_ARG_INFO(0, parameter)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_methodargs_methodoptionaldoublevaluewithdefaultstaticconstantaccess, 0, 0, 0)
	ZEND_ARG_INFO(0, parameter)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_methodargs_methodoptionalboolvaluewithdefaultstaticconstantaccess, 0, 0, 0)
	ZEND_ARG_INFO(0, parameter)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_methodargs_methodoptionalintvaluewithdefaultstaticconstantaccess, 0, 0, 0)
	ZEND_ARG_INFO(0, parameter)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_methodargs_method_entry) {
	PHP_ME(Test_MethodArgs, setCallable, arginfo_test_methodargs_setcallable, ZEND_ACC_PUBLIC)
	PHP_ME(Test_MethodArgs, setObject, arginfo_test_methodargs_setobject, ZEND_ACC_PUBLIC)
	PHP_ME(Test_MethodArgs, setCallableStrict, arginfo_test_methodargs_setcallablestrict, ZEND_ACC_PUBLIC)
	PHP_ME(Test_MethodArgs, setObjectStrict, arginfo_test_methodargs_setobjectstrict, ZEND_ACC_PUBLIC)
	PHP_ME(Test_MethodArgs, setResourceStrict, arginfo_test_methodargs_setresourcestrict, ZEND_ACC_PUBLIC)
	PHP_ME(Test_MethodArgs, methodOptionalValueWithDefaultStaticConstantAccess, arginfo_test_methodargs_methodoptionalvaluewithdefaultstaticconstantaccess, ZEND_ACC_PUBLIC)
	PHP_ME(Test_MethodArgs, methodOptionalStringValueWithDefaultStaticConstantAccess, arginfo_test_methodargs_methodoptionalstringvaluewithdefaultstaticconstantaccess, ZEND_ACC_PUBLIC)
	PHP_ME(Test_MethodArgs, methodOptionalDoubleValueWithDefaultStaticConstantAccess, arginfo_test_methodargs_methodoptionaldoublevaluewithdefaultstaticconstantaccess, ZEND_ACC_PUBLIC)
	PHP_ME(Test_MethodArgs, methodOptionalBoolValueWithDefaultStaticConstantAccess, arginfo_test_methodargs_methodoptionalboolvaluewithdefaultstaticconstantaccess, ZEND_ACC_PUBLIC)
	PHP_ME(Test_MethodArgs, methodOptionalIntValueWithDefaultStaticConstantAccess, arginfo_test_methodargs_methodoptionalintvaluewithdefaultstaticconstantaccess, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
