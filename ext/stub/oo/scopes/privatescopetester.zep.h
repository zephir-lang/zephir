
extern zend_class_entry *stub_oo_scopes_privatescopetester_ce;

ZEPHIR_INIT_CLASS(Stub_Oo_Scopes_PrivateScopeTester);

PHP_METHOD(Stub_Oo_Scopes_PrivateScopeTester, run);
PHP_METHOD(Stub_Oo_Scopes_PrivateScopeTester, setPropertyObj);
PHP_METHOD(Stub_Oo_Scopes_PrivateScopeTester, setPropertyNew);
PHP_METHOD(Stub_Oo_Scopes_PrivateScopeTester, getObjVars);
PHP_METHOD(Stub_Oo_Scopes_PrivateScopeTester, getNewVars);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_oo_scopes_privatescopetester_run, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_scopes_privatescopetester_setpropertyobj, 0, 0, 3)
	ZEND_ARG_INFO(0, obj)
	ZEND_ARG_TYPE_INFO(0, property, IS_STRING, 0)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_scopes_privatescopetester_setpropertynew, 0, 0, 3)
	ZEND_ARG_TYPE_INFO(0, className, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, property, IS_STRING, 0)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_scopes_privatescopetester_getobjvars, 0, 0, 1)
	ZEND_ARG_INFO(0, obj)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_scopes_privatescopetester_getnewvars, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, className, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_oo_scopes_privatescopetester_method_entry) {
	PHP_ME(Stub_Oo_Scopes_PrivateScopeTester, run, arginfo_stub_oo_scopes_privatescopetester_run, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_Scopes_PrivateScopeTester, setPropertyObj, arginfo_stub_oo_scopes_privatescopetester_setpropertyobj, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_Scopes_PrivateScopeTester, setPropertyNew, arginfo_stub_oo_scopes_privatescopetester_setpropertynew, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_Scopes_PrivateScopeTester, getObjVars, arginfo_stub_oo_scopes_privatescopetester_getobjvars, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_Scopes_PrivateScopeTester, getNewVars, arginfo_stub_oo_scopes_privatescopetester_getnewvars, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
