
extern zend_class_entry *stub_issue2826_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2826);

PHP_METHOD(Stub_Issue2826, fetchDynamic);
PHP_METHOD(Stub_Issue2826, fetchLong);
PHP_METHOD(Stub_Issue2826, fetchString);
PHP_METHOD(Stub_Issue2826, emptyDynamic);
PHP_METHOD(Stub_Issue2826, emptyLong);
PHP_METHOD(Stub_Issue2826, emptyString);
PHP_METHOD(Stub_Issue2826, assignFetched);
PHP_METHOD(Stub_Issue2826, appendFetched);
PHP_METHOD(Stub_Issue2826, unsetFetched);
PHP_METHOD(Stub_Issue2826, assignNestedFetched);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2826_fetchdynamic, 0, 2, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, params)
	ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2826_fetchlong, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, params)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2826_fetchstring, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, params)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2826_emptydynamic, 0, 2, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, params)
	ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2826_emptylong, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, params)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2826_emptystring, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, params)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2826_assignfetched, 0, 0, 1)
	ZEND_ARG_INFO(0, params)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2826_appendfetched, 0, 0, 1)
	ZEND_ARG_INFO(0, params)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2826_unsetfetched, 0, 0, 1)
	ZEND_ARG_INFO(0, params)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2826_assignnestedfetched, 0, 0, 1)
	ZEND_ARG_INFO(0, params)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2826_method_entry) {
	PHP_ME(Stub_Issue2826, fetchDynamic, arginfo_stub_issue2826_fetchdynamic, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2826, fetchLong, arginfo_stub_issue2826_fetchlong, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2826, fetchString, arginfo_stub_issue2826_fetchstring, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2826, emptyDynamic, arginfo_stub_issue2826_emptydynamic, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2826, emptyLong, arginfo_stub_issue2826_emptylong, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2826, emptyString, arginfo_stub_issue2826_emptystring, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2826, assignFetched, arginfo_stub_issue2826_assignfetched, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2826, appendFetched, arginfo_stub_issue2826_appendfetched, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2826, unsetFetched, arginfo_stub_issue2826_unsetfetched, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2826, assignNestedFetched, arginfo_stub_issue2826_assignnestedfetched, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
