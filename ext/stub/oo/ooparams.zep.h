
extern zend_class_entry *stub_oo_ooparams_ce;

ZEPHIR_INIT_CLASS(Stub_Oo_OoParams);

PHP_METHOD(Stub_Oo_OoParams, createThisClassWithoutWriteCurrentNamespace);
PHP_METHOD(Stub_Oo_OoParams, createOtherClassWithoutWriteCurrentNamespace);
PHP_METHOD(Stub_Oo_OoParams, setAge);
PHP_METHOD(Stub_Oo_OoParams, setAverage);
PHP_METHOD(Stub_Oo_OoParams, setName);
PHP_METHOD(Stub_Oo_OoParams, setEnabled);
PHP_METHOD(Stub_Oo_OoParams, setList);
PHP_METHOD(Stub_Oo_OoParams, setStrictAge);
PHP_METHOD(Stub_Oo_OoParams, setStrictAverage);
PHP_METHOD(Stub_Oo_OoParams, setStrictNameFromZephirLand);
PHP_METHOD(Stub_Oo_OoParams, setStrictName);
PHP_METHOD(Stub_Oo_OoParams, setStrictEnabled);
PHP_METHOD(Stub_Oo_OoParams, setStrictList);
PHP_METHOD(Stub_Oo_OoParams, setAgeDefault);
PHP_METHOD(Stub_Oo_OoParams, setAverageDefault);
PHP_METHOD(Stub_Oo_OoParams, setNameDefault);
PHP_METHOD(Stub_Oo_OoParams, setEnabledDefault);
PHP_METHOD(Stub_Oo_OoParams, setListDefault);
PHP_METHOD(Stub_Oo_OoParams, setConstAge);
PHP_METHOD(Stub_Oo_OoParams, setConstAverage);
PHP_METHOD(Stub_Oo_OoParams, setObject);
PHP_METHOD(Stub_Oo_OoParams, setCallable);
PHP_METHOD(Stub_Oo_OoParams, setResource);
PHP_METHOD(Stub_Oo_OoParams, setObjectClassCast);
PHP_METHOD(Stub_Oo_OoParams, getDefaultParamValue);
PHP_METHOD(Stub_Oo_OoParams, getDefaultParamValue1);
PHP_METHOD(Stub_Oo_OoParams, getDefaultParamValueStr);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_ooparams_createthisclasswithoutwritecurrentnamespace, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_ooparams_createotherclasswithoutwritecurrentnamespace, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_ooparams_setage, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, age, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_ooparams_setaverage, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, average, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_ooparams_setname, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_ooparams_setenabled, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, enabled, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_ooparams_setlist, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, someList, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_ooparams_setstrictage, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, age, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_ooparams_setstrictaverage, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, average, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_ooparams_setstrictnamefromzephirland, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_oo_ooparams_setstrictname, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_ooparams_setstrictenabled, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, enabled, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_ooparams_setstrictlist, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, someList, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_ooparams_setagedefault, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, age, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_ooparams_setaveragedefault, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, average, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_ooparams_setnamedefault, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_ooparams_setenableddefault, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, enabled, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_ooparams_setlistdefault, 0, 0, 0)
ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, someList, IS_ARRAY, 0, "[]")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_ooparams_setconstage, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, age, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_ooparams_setconstaverage, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, average, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_ooparams_setobject, 0, 0, 1)
	ZEND_ARG_INFO(0, obj)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_ooparams_setcallable, 0, 0, 1)
	ZEND_ARG_INFO(0, obj)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_ooparams_setresource, 0, 0, 1)
	ZEND_ARG_INFO(0, obj)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_ooparams_setobjectclasscast, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, parameter, Stub\\Oo\\Param, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_ooparams_getdefaultparamvalue, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, i, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_ooparams_getdefaultparamvalue1, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, i, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_ooparams_getdefaultparamvaluestr, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_oo_ooparams_method_entry) {
PHP_ME(Stub_Oo_OoParams, createThisClassWithoutWriteCurrentNamespace, arginfo_stub_oo_ooparams_createthisclasswithoutwritecurrentnamespace, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Oo_OoParams, createOtherClassWithoutWriteCurrentNamespace, arginfo_stub_oo_ooparams_createotherclasswithoutwritecurrentnamespace, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_OoParams, setAge, arginfo_stub_oo_ooparams_setage, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_OoParams, setAverage, arginfo_stub_oo_ooparams_setaverage, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_OoParams, setName, arginfo_stub_oo_ooparams_setname, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_OoParams, setEnabled, arginfo_stub_oo_ooparams_setenabled, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_OoParams, setList, arginfo_stub_oo_ooparams_setlist, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_OoParams, setStrictAge, arginfo_stub_oo_ooparams_setstrictage, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_OoParams, setStrictAverage, arginfo_stub_oo_ooparams_setstrictaverage, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Oo_OoParams, setStrictNameFromZephirLand, arginfo_stub_oo_ooparams_setstrictnamefromzephirland, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_OoParams, setStrictName, arginfo_stub_oo_ooparams_setstrictname, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_OoParams, setStrictEnabled, arginfo_stub_oo_ooparams_setstrictenabled, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_OoParams, setStrictList, arginfo_stub_oo_ooparams_setstrictlist, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_OoParams, setAgeDefault, arginfo_stub_oo_ooparams_setagedefault, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_OoParams, setAverageDefault, arginfo_stub_oo_ooparams_setaveragedefault, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_OoParams, setNameDefault, arginfo_stub_oo_ooparams_setnamedefault, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_OoParams, setEnabledDefault, arginfo_stub_oo_ooparams_setenableddefault, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_OoParams, setListDefault, arginfo_stub_oo_ooparams_setlistdefault, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_OoParams, setConstAge, arginfo_stub_oo_ooparams_setconstage, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_OoParams, setConstAverage, arginfo_stub_oo_ooparams_setconstaverage, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_OoParams, setObject, arginfo_stub_oo_ooparams_setobject, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_OoParams, setCallable, arginfo_stub_oo_ooparams_setcallable, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_OoParams, setResource, arginfo_stub_oo_ooparams_setresource, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_OoParams, setObjectClassCast, arginfo_stub_oo_ooparams_setobjectclasscast, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_OoParams, getDefaultParamValue, arginfo_stub_oo_ooparams_getdefaultparamvalue, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_OoParams, getDefaultParamValue1, arginfo_stub_oo_ooparams_getdefaultparamvalue1, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_OoParams, getDefaultParamValueStr, arginfo_stub_oo_ooparams_getdefaultparamvaluestr, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
