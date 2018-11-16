
extern zend_class_entry *test_oo_scopes_scopetesterinterface_ce;

ZEPHIR_INIT_CLASS(Test_Oo_Scopes_ScopeTesterInterface);

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_oo_scopes_scopetesterinterface_run, 0, 0, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_oo_scopes_scopetesterinterface_run, 0, 0, IS_STRING, NULL, 0)
#endif
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_oo_scopes_scopetesterinterface_method_entry) {
	PHP_ABSTRACT_ME(Test_Oo_Scopes_ScopeTesterInterface, run, arginfo_test_oo_scopes_scopetesterinterface_run)
	PHP_FE_END
};
