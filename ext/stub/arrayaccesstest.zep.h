
extern zend_class_entry *stub_arrayaccesstest_ce;

ZEPHIR_INIT_CLASS(Stub_ArrayAccessTest);

PHP_METHOD(Stub_ArrayAccessTest, exits);
PHP_METHOD(Stub_ArrayAccessTest, get);
PHP_METHOD(Stub_ArrayAccessTest, issue645);
PHP_METHOD(Stub_ArrayAccessTest, issue1155);
PHP_METHOD(Stub_ArrayAccessTest, issue1094Test1);
PHP_METHOD(Stub_ArrayAccessTest, issue1094Test2);
PHP_METHOD(Stub_ArrayAccessTest, issue1094Test3);
PHP_METHOD(Stub_ArrayAccessTest, issue1086AddElementToArrayWithStrictParams);
PHP_METHOD(Stub_ArrayAccessTest, issue1086CanAddAnElementToExistingArrayWithStrictParams);
PHP_METHOD(Stub_ArrayAccessTest, issue1086AddElementToArrayWithoutStrictParams);
PHP_METHOD(Stub_ArrayAccessTest, issue1086CanAddAnElementToExistingArrayWithoutStrictParams);

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

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_arrayaccesstest_issue1086addelementtoarraywithstrictparams, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, params, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_arrayaccesstest_issue1086canaddanelementtoexistingarraywithstrictparams, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_arrayaccesstest_issue1086addelementtoarraywithoutstrictparams, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, params, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_arrayaccesstest_issue1086canaddanelementtoexistingarraywithoutstrictparams, 0, 0, IS_ARRAY, 0)
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
	PHP_ME(Stub_ArrayAccessTest, issue1086AddElementToArrayWithStrictParams, arginfo_stub_arrayaccesstest_issue1086addelementtoarraywithstrictparams, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_ArrayAccessTest, issue1086CanAddAnElementToExistingArrayWithStrictParams, arginfo_stub_arrayaccesstest_issue1086canaddanelementtoexistingarraywithstrictparams, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_ArrayAccessTest, issue1086AddElementToArrayWithoutStrictParams, arginfo_stub_arrayaccesstest_issue1086addelementtoarraywithoutstrictparams, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_ArrayAccessTest, issue1086CanAddAnElementToExistingArrayWithoutStrictParams, arginfo_stub_arrayaccesstest_issue1086canaddanelementtoexistingarraywithoutstrictparams, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
