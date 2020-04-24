
extern zend_class_entry *test_oo_scopes_abstractclassmagic_ce;

ZEPHIR_INIT_CLASS(Test_Oo_Scopes_AbstractClassMagic);

PHP_METHOD(Test_Oo_Scopes_AbstractClassMagic, __set);
PHP_METHOD(Test_Oo_Scopes_AbstractClassMagic, __get);

#if PHP_VERSION_ID >= 70100
#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_oo_scopes_abstractclassmagic___set, 0, 2, IS_VOID, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_oo_scopes_abstractclassmagic___set, 0, 2, IS_VOID, NULL, 0)
#endif
#else
ZEND_BEGIN_ARG_INFO_EX(arginfo_test_oo_scopes_abstractclassmagic___set, 0, 0, 2)
#define arginfo_test_oo_scopes_abstractclassmagic___set NULL
#endif

#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
#else
	ZEND_ARG_INFO(0, name)
#endif
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_oo_scopes_abstractclassmagic___get, 0, 0, 1)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
#else
	ZEND_ARG_INFO(0, name)
#endif
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_oo_scopes_abstractclassmagic_method_entry) {
	PHP_ME(Test_Oo_Scopes_AbstractClassMagic, __set, arginfo_test_oo_scopes_abstractclassmagic___set, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_Scopes_AbstractClassMagic, __get, arginfo_test_oo_scopes_abstractclassmagic___get, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
