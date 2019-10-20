
extern zend_class_entry *test_spropertyaccess_ce;

ZEPHIR_INIT_CLASS(Test_SPropertyAccess);

PHP_METHOD(Test_SPropertyAccess, __construct);
PHP_METHOD(Test_SPropertyAccess, testArgumentWithUnderscore);
PHP_METHOD(Test_SPropertyAccess, testArgument);
PHP_METHOD(Test_SPropertyAccess, mutateStringVarInsideCycle);
PHP_METHOD(Test_SPropertyAccess, mutateIntVarInsideCycle);
PHP_METHOD(Test_SPropertyAccess, mutateDoubleVarInsideCycle);
PHP_METHOD(Test_SPropertyAccess, mutateArrayVarInsideCycle);
void zephir_init_static_properties_Test_SPropertyAccess(TSRMLS_D);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_spropertyaccess_testargumentwithunderscore, 0, 0, 0)
	ZEND_ARG_INFO(0, delimiter)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_spropertyaccess_testargument, 0, 0, 0)
	ZEND_ARG_INFO(0, delimiter)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_spropertyaccess_mutatestringvarinsidecycle, 0, 0, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_spropertyaccess_mutatestringvarinsidecycle, 0, 0, IS_STRING, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_spropertyaccess_mutateintvarinsidecycle, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_spropertyaccess_mutateintvarinsidecycle, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_spropertyaccess_mutatedoublevarinsidecycle, 0, 0, IS_DOUBLE, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_spropertyaccess_mutatedoublevarinsidecycle, 0, 0, IS_DOUBLE, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_spropertyaccess_mutatearrayvarinsidecycle, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_spropertyaccess_mutatearrayvarinsidecycle, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_spropertyaccess_method_entry) {
	PHP_ME(Test_SPropertyAccess, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Test_SPropertyAccess, testArgumentWithUnderscore, arginfo_test_spropertyaccess_testargumentwithunderscore, ZEND_ACC_PUBLIC)
	PHP_ME(Test_SPropertyAccess, testArgument, arginfo_test_spropertyaccess_testargument, ZEND_ACC_PUBLIC)
	PHP_ME(Test_SPropertyAccess, mutateStringVarInsideCycle, arginfo_test_spropertyaccess_mutatestringvarinsidecycle, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Test_SPropertyAccess, mutateIntVarInsideCycle, arginfo_test_spropertyaccess_mutateintvarinsidecycle, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Test_SPropertyAccess, mutateDoubleVarInsideCycle, arginfo_test_spropertyaccess_mutatedoublevarinsidecycle, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Test_SPropertyAccess, mutateArrayVarInsideCycle, arginfo_test_spropertyaccess_mutatearrayvarinsidecycle, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
