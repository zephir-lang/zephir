
extern zend_class_entry *stub_spropertyaccess_ce;

ZEPHIR_INIT_CLASS(Stub_SPropertyAccess);

PHP_METHOD(Stub_SPropertyAccess, __construct);
PHP_METHOD(Stub_SPropertyAccess, testArgumentWithUnderscore);
PHP_METHOD(Stub_SPropertyAccess, testArgument);
PHP_METHOD(Stub_SPropertyAccess, mutateStringVarInsideCycle);
PHP_METHOD(Stub_SPropertyAccess, mutateIntVarInsideCycle);
PHP_METHOD(Stub_SPropertyAccess, mutateDoubleVarInsideCycle);
PHP_METHOD(Stub_SPropertyAccess, mutateArrayVarInsideCycle);
void zephir_init_static_properties_Stub_SPropertyAccess();

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_spropertyaccess___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_spropertyaccess_testargumentwithunderscore, 0, 0, 0)
	ZEND_ARG_INFO(0, delimiter)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_spropertyaccess_testargument, 0, 0, 0)
	ZEND_ARG_INFO(0, delimiter)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_spropertyaccess_mutatestringvarinsidecycle, 0, 0, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_spropertyaccess_mutatestringvarinsidecycle, 0, 0, IS_STRING, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_spropertyaccess_mutateintvarinsidecycle, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_spropertyaccess_mutateintvarinsidecycle, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_spropertyaccess_mutatedoublevarinsidecycle, 0, 0, IS_DOUBLE, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_spropertyaccess_mutatedoublevarinsidecycle, 0, 0, IS_DOUBLE, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_spropertyaccess_mutatearrayvarinsidecycle, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_spropertyaccess_mutatearrayvarinsidecycle, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_spropertyaccess_zephir_init_static_properties_stub_spropertyaccess, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_spropertyaccess_method_entry) {
	PHP_ME(Stub_SPropertyAccess, __construct, arginfo_stub_spropertyaccess___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Stub_SPropertyAccess, testArgumentWithUnderscore, arginfo_stub_spropertyaccess_testargumentwithunderscore, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_SPropertyAccess, testArgument, arginfo_stub_spropertyaccess_testargument, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_SPropertyAccess, mutateStringVarInsideCycle, arginfo_stub_spropertyaccess_mutatestringvarinsidecycle, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_SPropertyAccess, mutateIntVarInsideCycle, arginfo_stub_spropertyaccess_mutateintvarinsidecycle, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_SPropertyAccess, mutateDoubleVarInsideCycle, arginfo_stub_spropertyaccess_mutatedoublevarinsidecycle, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_SPropertyAccess, mutateArrayVarInsideCycle, arginfo_stub_spropertyaccess_mutatearrayvarinsidecycle, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
