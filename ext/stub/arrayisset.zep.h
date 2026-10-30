
extern zend_class_entry *stub_arrayisset_ce;

ZEPHIR_INIT_CLASS(Stub_ArrayIsset);

PHP_METHOD(Stub_ArrayIsset, issetVar);
PHP_METHOD(Stub_ArrayIsset, emptyVar);
PHP_METHOD(Stub_ArrayIsset, fetchVar);
PHP_METHOD(Stub_ArrayIsset, issetStringLiteral);
PHP_METHOD(Stub_ArrayIsset, issetIntLiteral);
PHP_METHOD(Stub_ArrayIsset, issetNested);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_arrayisset_issetvar, 0, 2, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, k)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_arrayisset_emptyvar, 0, 2, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, k)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_arrayisset_fetchvar, 0, 2, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, k)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_arrayisset_issetstringliteral, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_arrayisset_issetintliteral, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_arrayisset_issetnested, 0, 3, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, i)
	ZEND_ARG_INFO(0, j)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_arrayisset_method_entry) {
	PHP_ME(Stub_ArrayIsset, issetVar, arginfo_stub_arrayisset_issetvar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayIsset, emptyVar, arginfo_stub_arrayisset_emptyvar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayIsset, fetchVar, arginfo_stub_arrayisset_fetchvar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayIsset, issetStringLiteral, arginfo_stub_arrayisset_issetstringliteral, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayIsset, issetIntLiteral, arginfo_stub_arrayisset_issetintliteral, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayIsset, issetNested, arginfo_stub_arrayisset_issetnested, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
