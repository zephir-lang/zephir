
extern zend_class_entry *stub_issue2799_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2799);

PHP_METHOD(Stub_Issue2799, getItems);
PHP_METHOD(Stub_Issue2799, getLiteral);
PHP_METHOD(Stub_Issue2799, getWithConst);
PHP_METHOD(Stub_Issue2799, append);
PHP_METHOD(Stub_Issue2799, setItem);
PHP_METHOD(Stub_Issue2799, unsetItem);
PHP_METHOD(Stub_Issue2799, setKey);
PHP_METHOD(Stub_Issue2799, appendNested);
PHP_METHOD(Stub_Issue2799, appendNestedEmpty);
PHP_METHOD(Stub_Issue2799, unsetKey);
PHP_METHOD(Stub_Issue2799, incrementF);
PHP_METHOD(Stub_Issue2799, shift);
PHP_METHOD(Stub_Issue2799, appendConst);
PHP_METHOD(Stub_Issue2799, appendTyped);
zend_object *zephir_init_properties_Stub_Issue2799(zend_class_entry *class_type);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2799_getitems, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2799_getliteral, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2799_getwithconst, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2799_append, 0, 1, IS_VOID, 0)

	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2799_setitem, 0, 2, IS_VOID, 0)

	ZEND_ARG_INFO(0, k)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2799_unsetitem, 0, 1, IS_VOID, 0)

	ZEND_ARG_INFO(0, k)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2799_setkey, 0, 2, IS_VOID, 0)

	ZEND_ARG_INFO(0, k)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2799_appendnested, 0, 1, IS_VOID, 0)

	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2799_appendnestedempty, 0, 1, IS_VOID, 0)

	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2799_unsetkey, 0, 1, IS_VOID, 0)

	ZEND_ARG_INFO(0, k)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2799_incrementf, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2799_shift, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2799_appendconst, 0, 1, IS_VOID, 0)

	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2799_appendtyped, 0, 1, IS_VOID, 0)

	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2799_zephir_init_properties_stub_issue2799, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2799_method_entry) {
PHP_ME(Stub_Issue2799, getItems, arginfo_stub_issue2799_getitems, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Issue2799, getLiteral, arginfo_stub_issue2799_getliteral, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Issue2799, getWithConst, arginfo_stub_issue2799_getwithconst, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2799, append, arginfo_stub_issue2799_append, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2799, setItem, arginfo_stub_issue2799_setitem, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2799, unsetItem, arginfo_stub_issue2799_unsetitem, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2799, setKey, arginfo_stub_issue2799_setkey, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2799, appendNested, arginfo_stub_issue2799_appendnested, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2799, appendNestedEmpty, arginfo_stub_issue2799_appendnestedempty, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2799, unsetKey, arginfo_stub_issue2799_unsetkey, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2799, incrementF, arginfo_stub_issue2799_incrementf, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Issue2799, shift, arginfo_stub_issue2799_shift, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2799, appendConst, arginfo_stub_issue2799_appendconst, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2799, appendTyped, arginfo_stub_issue2799_appendtyped, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
