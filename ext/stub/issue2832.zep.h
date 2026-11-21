
extern zend_class_entry *stub_issue2832_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2832);

PHP_METHOD(Stub_Issue2832, fetchName);
PHP_METHOD(Stub_Issue2832, fetchLiteral);
PHP_METHOD(Stub_Issue2832, fetchString);
PHP_METHOD(Stub_Issue2832, fetchEscaped);
PHP_METHOD(Stub_Issue2832, issetEscaped);
PHP_METHOD(Stub_Issue2832, emptyEscaped);
PHP_METHOD(Stub_Issue2832, readEscaped);
PHP_METHOD(Stub_Issue2832, writeEscaped);
PHP_METHOD(Stub_Issue2832, fetchIntName);
PHP_METHOD(Stub_Issue2832, fetchStatement);
PHP_METHOD(Stub_Issue2832, fetchThenMark);
PHP_METHOD(Stub_Issue2832, fetchSelf);
PHP_METHOD(Stub_Issue2832, fetchIntoName);
PHP_METHOD(Stub_Issue2832, fetchSelfArray);
PHP_METHOD(Stub_Issue2832, fetchIntoKey);
PHP_METHOD(Stub_Issue2832, issetKey);
PHP_METHOD(Stub_Issue2832, fetchKey);
PHP_METHOD(Stub_Issue2832, emptyKey);
PHP_METHOD(Stub_Issue2832, issetDeep);
PHP_METHOD(Stub_Issue2832, fetchDeep);
PHP_METHOD(Stub_Issue2832, emptyDeep);
PHP_METHOD(Stub_Issue2832, issetPropDeep);
PHP_METHOD(Stub_Issue2832, fetchPropDeep);
PHP_METHOD(Stub_Issue2832, issetDynamicDeep);
PHP_METHOD(Stub_Issue2832, issetPropArray);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2832_fetchname, 0, 2, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, o)
	ZEND_ARG_INFO(0, n)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2832_fetchliteral, 0, 1, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, o)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2832_fetchstring, 0, 1, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, o)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2832_fetchescaped, 0, 1, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, o)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2832_issetescaped, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, o)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2832_emptyescaped, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, o)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2832_readescaped, 0, 0, 1)
	ZEND_ARG_INFO(0, o)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2832_writeescaped, 0, 2, IS_VOID, 0)

	ZEND_ARG_INFO(0, o)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2832_fetchintname, 0, 2, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, o)
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2832_fetchstatement, 0, 0, 2)
	ZEND_ARG_INFO(0, o)
	ZEND_ARG_INFO(0, n)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2832_fetchthenmark, 0, 0, 3)
	ZEND_ARG_INFO(0, o)
	ZEND_ARG_INFO(0, n)
	ZEND_ARG_INFO(0, target)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2832_fetchself, 0, 2, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, o)
	ZEND_ARG_INFO(0, n)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2832_fetchintoname, 0, 2, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, o)
	ZEND_ARG_INFO(0, n)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2832_fetchselfarray, 0, 2, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, k)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2832_fetchintokey, 0, 2, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, k)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2832_issetkey, 0, 2, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, o)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2832_fetchkey, 0, 2, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, o)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2832_emptykey, 0, 2, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, o)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2832_issetdeep, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2832_fetchdeep, 0, 1, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2832_emptydeep, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2832_issetpropdeep, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, o)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2832_fetchpropdeep, 0, 1, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, o)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2832_issetdynamicdeep, 0, 2, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, o)
	ZEND_ARG_INFO(0, n)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2832_issetproparray, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, o)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2832_method_entry) {
	PHP_ME(Stub_Issue2832, fetchName, arginfo_stub_issue2832_fetchname, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2832, fetchLiteral, arginfo_stub_issue2832_fetchliteral, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2832, fetchString, arginfo_stub_issue2832_fetchstring, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2832, fetchEscaped, arginfo_stub_issue2832_fetchescaped, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2832, issetEscaped, arginfo_stub_issue2832_issetescaped, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2832, emptyEscaped, arginfo_stub_issue2832_emptyescaped, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2832, readEscaped, arginfo_stub_issue2832_readescaped, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2832, writeEscaped, arginfo_stub_issue2832_writeescaped, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2832, fetchIntName, arginfo_stub_issue2832_fetchintname, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2832, fetchStatement, arginfo_stub_issue2832_fetchstatement, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2832, fetchThenMark, arginfo_stub_issue2832_fetchthenmark, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2832, fetchSelf, arginfo_stub_issue2832_fetchself, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2832, fetchIntoName, arginfo_stub_issue2832_fetchintoname, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2832, fetchSelfArray, arginfo_stub_issue2832_fetchselfarray, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2832, fetchIntoKey, arginfo_stub_issue2832_fetchintokey, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2832, issetKey, arginfo_stub_issue2832_issetkey, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2832, fetchKey, arginfo_stub_issue2832_fetchkey, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2832, emptyKey, arginfo_stub_issue2832_emptykey, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2832, issetDeep, arginfo_stub_issue2832_issetdeep, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2832, fetchDeep, arginfo_stub_issue2832_fetchdeep, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2832, emptyDeep, arginfo_stub_issue2832_emptydeep, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2832, issetPropDeep, arginfo_stub_issue2832_issetpropdeep, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2832, fetchPropDeep, arginfo_stub_issue2832_fetchpropdeep, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2832, issetDynamicDeep, arginfo_stub_issue2832_issetdynamicdeep, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2832, issetPropArray, arginfo_stub_issue2832_issetproparray, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
