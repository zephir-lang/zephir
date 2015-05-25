
extern zend_class_entry *test_oo_ooparams_ce;

ZEPHIR_INIT_CLASS(Test_Oo_OoParams);

PHP_METHOD(Test_Oo_OoParams, createThisClassWithoutWriteCurrentNamespace);
PHP_METHOD(Test_Oo_OoParams, createOtherClassWithoutWriteCurrentNamespace);
PHP_METHOD(Test_Oo_OoParams, setAge);
PHP_METHOD(Test_Oo_OoParams, setAverage);
PHP_METHOD(Test_Oo_OoParams, setName);
PHP_METHOD(Test_Oo_OoParams, setEnabled);
PHP_METHOD(Test_Oo_OoParams, setList);
PHP_METHOD(Test_Oo_OoParams, setStrictAge);
PHP_METHOD(Test_Oo_OoParams, setStrictAverage);
PHP_METHOD(Test_Oo_OoParams, setStrictName);
PHP_METHOD(Test_Oo_OoParams, setStrictEnabled);
PHP_METHOD(Test_Oo_OoParams, setStrictList);
PHP_METHOD(Test_Oo_OoParams, setAgeDefault);
PHP_METHOD(Test_Oo_OoParams, setAverageDefault);
PHP_METHOD(Test_Oo_OoParams, setNameDefault);
PHP_METHOD(Test_Oo_OoParams, setEnabledDefault);
PHP_METHOD(Test_Oo_OoParams, setListDefault);
PHP_METHOD(Test_Oo_OoParams, setConstAge);
PHP_METHOD(Test_Oo_OoParams, setConstAverage);
PHP_METHOD(Test_Oo_OoParams, setObject);
PHP_METHOD(Test_Oo_OoParams, setCallable);
PHP_METHOD(Test_Oo_OoParams, setResource);
PHP_METHOD(Test_Oo_OoParams, setObjectClassCast);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_oo_ooparams_setage, 0, 0, 1)
	ZEND_ARG_INFO(0, age)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_oo_ooparams_setaverage, 0, 0, 1)
	ZEND_ARG_INFO(0, average)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_oo_ooparams_setname, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_oo_ooparams_setenabled, 0, 0, 1)
	ZEND_ARG_INFO(0, enabled)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_oo_ooparams_setlist, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, someList, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_oo_ooparams_setstrictage, 0, 0, 1)
	ZEND_ARG_INFO(0, age)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_oo_ooparams_setstrictaverage, 0, 0, 1)
	ZEND_ARG_INFO(0, average)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_oo_ooparams_setstrictname, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_oo_ooparams_setstrictenabled, 0, 0, 1)
	ZEND_ARG_INFO(0, enabled)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_oo_ooparams_setstrictlist, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, someList, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_oo_ooparams_setagedefault, 0, 0, 0)
	ZEND_ARG_INFO(0, age)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_oo_ooparams_setaveragedefault, 0, 0, 0)
	ZEND_ARG_INFO(0, average)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_oo_ooparams_setnamedefault, 0, 0, 0)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_oo_ooparams_setenableddefault, 0, 0, 0)
	ZEND_ARG_INFO(0, enabled)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_oo_ooparams_setlistdefault, 0, 0, 0)
	ZEND_ARG_ARRAY_INFO(0, someList, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_oo_ooparams_setconstage, 0, 0, 1)
	ZEND_ARG_INFO(0, age)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_oo_ooparams_setconstaverage, 0, 0, 1)
	ZEND_ARG_INFO(0, average)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_oo_ooparams_setobject, 0, 0, 1)
	ZEND_ARG_INFO(0, obj)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_oo_ooparams_setcallable, 0, 0, 1)
	ZEND_ARG_INFO(0, obj)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_oo_ooparams_setresource, 0, 0, 1)
	ZEND_ARG_INFO(0, obj)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_oo_ooparams_setobjectclasscast, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, parameter, Test\\Oo\\Param, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_oo_ooparams_method_entry) {
	PHP_ME(Test_Oo_OoParams, createThisClassWithoutWriteCurrentNamespace, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_OoParams, createOtherClassWithoutWriteCurrentNamespace, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_OoParams, setAge, arginfo_test_oo_ooparams_setage, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_OoParams, setAverage, arginfo_test_oo_ooparams_setaverage, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_OoParams, setName, arginfo_test_oo_ooparams_setname, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_OoParams, setEnabled, arginfo_test_oo_ooparams_setenabled, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_OoParams, setList, arginfo_test_oo_ooparams_setlist, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_OoParams, setStrictAge, arginfo_test_oo_ooparams_setstrictage, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_OoParams, setStrictAverage, arginfo_test_oo_ooparams_setstrictaverage, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_OoParams, setStrictName, arginfo_test_oo_ooparams_setstrictname, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_OoParams, setStrictEnabled, arginfo_test_oo_ooparams_setstrictenabled, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_OoParams, setStrictList, arginfo_test_oo_ooparams_setstrictlist, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_OoParams, setAgeDefault, arginfo_test_oo_ooparams_setagedefault, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_OoParams, setAverageDefault, arginfo_test_oo_ooparams_setaveragedefault, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_OoParams, setNameDefault, arginfo_test_oo_ooparams_setnamedefault, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_OoParams, setEnabledDefault, arginfo_test_oo_ooparams_setenableddefault, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_OoParams, setListDefault, arginfo_test_oo_ooparams_setlistdefault, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_OoParams, setConstAge, arginfo_test_oo_ooparams_setconstage, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_OoParams, setConstAverage, arginfo_test_oo_ooparams_setconstaverage, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_OoParams, setObject, arginfo_test_oo_ooparams_setobject, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_OoParams, setCallable, arginfo_test_oo_ooparams_setcallable, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_OoParams, setResource, arginfo_test_oo_ooparams_setresource, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_OoParams, setObjectClassCast, arginfo_test_oo_ooparams_setobjectclasscast, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
