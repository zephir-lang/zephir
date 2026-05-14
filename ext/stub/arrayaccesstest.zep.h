
extern zend_class_entry *stub_arrayaccesstest_ce;

ZEPHIR_INIT_CLASS(Stub_ArrayAccessTest);

PHP_METHOD(Stub_ArrayAccessTest, exits);
PHP_METHOD(Stub_ArrayAccessTest, get);
PHP_METHOD(Stub_ArrayAccessTest, unsetByKeyFromArray);
PHP_METHOD(Stub_ArrayAccessTest, unsetByKeyFromProperty);
PHP_METHOD(Stub_ArrayAccessTest, issue645);
PHP_METHOD(Stub_ArrayAccessTest, issue1155);
PHP_METHOD(Stub_ArrayAccessTest, issue1094Test1);
PHP_METHOD(Stub_ArrayAccessTest, issue1094Test2);
PHP_METHOD(Stub_ArrayAccessTest, issue1094Test3);
PHP_METHOD(Stub_ArrayAccessTest, issue1086Strict);
PHP_METHOD(Stub_ArrayAccessTest, issue1086WontNullArrayAfterPassViaStaticWithStrictParams);
PHP_METHOD(Stub_ArrayAccessTest, issue1086NotStrictParams);
PHP_METHOD(Stub_ArrayAccessTest, issue1086WontNullArrayAfterPassViaStaticWithoutStrictParams);
PHP_METHOD(Stub_ArrayAccessTest, issue1259UnsetKeyFromArrayInternalVariable);
PHP_METHOD(Stub_ArrayAccessTest, issue1259UnsetStringKeyFromArrayProperty);
PHP_METHOD(Stub_ArrayAccessTest, issue1259UnsetLongKeyFromArrayProperty);
zend_object *zephir_init_properties_Stub_ArrayAccessTest(zend_class_entry *class_type);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_arrayaccesstest_exits, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_arrayaccesstest_get, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_arrayaccesstest_unsetbykeyfromarray, 0, 2, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
	ZEND_ARG_ARRAY_INFO(0, data, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_arrayaccesstest_unsetbykeyfromproperty, 0, 2, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
	ZEND_ARG_ARRAY_INFO(0, dataFromProperty, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_arrayaccesstest_issue645, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_arrayaccesstest_issue1155, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_arrayaccesstest_issue1094test1, 0, 0, _IS_BOOL, 0)
ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, items, IS_ARRAY, 1, "[]")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_arrayaccesstest_issue1094test2, 0, 0, _IS_BOOL, 0)
ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, items, IS_ARRAY, 1, "[]")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_arrayaccesstest_issue1094test3, 0, 0, _IS_BOOL, 0)
ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, items, IS_ARRAY, 1, "[]")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_arrayaccesstest_issue1086strict, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, params, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_arrayaccesstest_issue1086wontnullarrayafterpassviastaticwithstrictparams, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_arrayaccesstest_issue1086notstrictparams, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, params, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_arrayaccesstest_issue1086wontnullarrayafterpassviastaticwithoutstrictparams, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_arrayaccesstest_issue1259unsetkeyfromarrayinternalvariable, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_arrayaccesstest_issue1259unsetstringkeyfromarrayproperty, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_arrayaccesstest_issue1259unsetlongkeyfromarrayproperty, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_arrayaccesstest_zephir_init_properties_stub_arrayaccesstest, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_arrayaccesstest_method_entry) {
PHP_ME(Stub_ArrayAccessTest, exits, arginfo_stub_arrayaccesstest_exits, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
PHP_ME(Stub_ArrayAccessTest, get, arginfo_stub_arrayaccesstest_get, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_ArrayAccessTest, unsetByKeyFromArray, arginfo_stub_arrayaccesstest_unsetbykeyfromarray, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayAccessTest, unsetByKeyFromProperty, arginfo_stub_arrayaccesstest_unsetbykeyfromproperty, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayAccessTest, issue645, arginfo_stub_arrayaccesstest_issue645, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayAccessTest, issue1155, arginfo_stub_arrayaccesstest_issue1155, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayAccessTest, issue1094Test1, arginfo_stub_arrayaccesstest_issue1094test1, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayAccessTest, issue1094Test2, arginfo_stub_arrayaccesstest_issue1094test2, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayAccessTest, issue1094Test3, arginfo_stub_arrayaccesstest_issue1094test3, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayAccessTest, issue1086Strict, arginfo_stub_arrayaccesstest_issue1086strict, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_ArrayAccessTest, issue1086WontNullArrayAfterPassViaStaticWithStrictParams, arginfo_stub_arrayaccesstest_issue1086wontnullarrayafterpassviastaticwithstrictparams, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_ArrayAccessTest, issue1086NotStrictParams, arginfo_stub_arrayaccesstest_issue1086notstrictparams, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_ArrayAccessTest, issue1086WontNullArrayAfterPassViaStaticWithoutStrictParams, arginfo_stub_arrayaccesstest_issue1086wontnullarrayafterpassviastaticwithoutstrictparams, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_ArrayAccessTest, issue1259UnsetKeyFromArrayInternalVariable, arginfo_stub_arrayaccesstest_issue1259unsetkeyfromarrayinternalvariable, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayAccessTest, issue1259UnsetStringKeyFromArrayProperty, arginfo_stub_arrayaccesstest_issue1259unsetstringkeyfromarrayproperty, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayAccessTest, issue1259UnsetLongKeyFromArrayProperty, arginfo_stub_arrayaccesstest_issue1259unsetlongkeyfromarrayproperty, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
