
extern zend_class_entry *stub_arrayaccesstest_ce;

ZEPHIR_INIT_CLASS(Stub_ArrayAccessTest);

PHP_METHOD(Stub_ArrayAccessTest, exits);
PHP_METHOD(Stub_ArrayAccessTest, get);
PHP_METHOD(Stub_ArrayAccessTest, issue645);
PHP_METHOD(Stub_ArrayAccessTest, issue1155);
PHP_METHOD(Stub_ArrayAccessTest, issue1094Test1);
PHP_METHOD(Stub_ArrayAccessTest, issue1094Test2);
PHP_METHOD(Stub_ArrayAccessTest, issue1094Test3);
PHP_METHOD(Stub_ArrayAccessTest, issue1086Test1ArrayStrict);
PHP_METHOD(Stub_ArrayAccessTest, issue1086TestArrayStrict);
PHP_METHOD(Stub_ArrayAccessTest, issue1086Test1Array);
PHP_METHOD(Stub_ArrayAccessTest, issue1086TestArray);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_arrayaccesstest_exits, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_arrayaccesstest_get, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_arrayaccesstest_issue645, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_arrayaccesstest_issue1155, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_arrayaccesstest_issue1094test1, 0, 0, _IS_BOOL, 0)
	ZEND_ARG_ARRAY_INFO(0, items, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_arrayaccesstest_issue1094test2, 0, 0, _IS_BOOL, 0)
	ZEND_ARG_ARRAY_INFO(0, items, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_arrayaccesstest_issue1094test3, 0, 0, _IS_BOOL, 0)
	ZEND_ARG_ARRAY_INFO(0, items, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_arrayaccesstest_issue1086test1arraystrict, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, params, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_arrayaccesstest_issue1086testarraystrict, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_arrayaccesstest_issue1086test1array, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, params, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_arrayaccesstest_issue1086testarray, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_arrayaccesstest_method_entry) {
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_ArrayAccessTest, exits, arginfo_stub_arrayaccesstest_exits, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
#else
	PHP_ME(Stub_ArrayAccessTest, exits, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_ArrayAccessTest, get, arginfo_stub_arrayaccesstest_get, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
#else
	PHP_ME(Stub_ArrayAccessTest, get, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
#endif
	PHP_ME(Stub_ArrayAccessTest, issue645, arginfo_stub_arrayaccesstest_issue645, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayAccessTest, issue1155, arginfo_stub_arrayaccesstest_issue1155, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayAccessTest, issue1094Test1, arginfo_stub_arrayaccesstest_issue1094test1, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayAccessTest, issue1094Test2, arginfo_stub_arrayaccesstest_issue1094test2, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayAccessTest, issue1094Test3, arginfo_stub_arrayaccesstest_issue1094test3, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayAccessTest, issue1086Test1ArrayStrict, arginfo_stub_arrayaccesstest_issue1086test1arraystrict, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_ArrayAccessTest, issue1086TestArrayStrict, arginfo_stub_arrayaccesstest_issue1086testarraystrict, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_ArrayAccessTest, issue1086Test1Array, arginfo_stub_arrayaccesstest_issue1086test1array, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_ArrayAccessTest, issue1086TestArray, arginfo_stub_arrayaccesstest_issue1086testarray, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
