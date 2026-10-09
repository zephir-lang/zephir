
extern zend_class_entry *stub_issue2705_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2705);

PHP_METHOD(Stub_Issue2705, unsetBraceLiteral);
PHP_METHOD(Stub_Issue2705, unsetBraceName);
PHP_METHOD(Stub_Issue2705, unsetBraceNameAndKey);
PHP_METHOD(Stub_Issue2705, unsetBraceNameLongKey);
PHP_METHOD(Stub_Issue2705, unsetNested);
PHP_METHOD(Stub_Issue2705, unsetNestedByVar);
PHP_METHOD(Stub_Issue2705, unsetDeep);
PHP_METHOD(Stub_Issue2705, unsetBraceLiteralNested);
PHP_METHOD(Stub_Issue2705, unsetBraceNameNested);
PHP_METHOD(Stub_Issue2705, unsetNestedThenAssign);
PHP_METHOD(Stub_Issue2705, unsetPropertyOffsetThenAssign);
PHP_METHOD(Stub_Issue2705, unsetLocalOffsetThenAssign);
PHP_METHOD(Stub_Issue2705, unsetPropertyThenAssign);
PHP_METHOD(Stub_Issue2705, unsetNamedPropertyThenAssign);
PHP_METHOD(Stub_Issue2705, unsetLocalNested);
PHP_METHOD(Stub_Issue2705, unsetLocalDeep);
PHP_METHOD(Stub_Issue2705, unsetStatic);
PHP_METHOD(Stub_Issue2705, unsetStaticNested);
PHP_METHOD(Stub_Issue2705, snapshotDefaults);
PHP_METHOD(Stub_Issue2705, snapshotStaticDefaults);
PHP_METHOD(Stub_Issue2705, nestedProbe);
PHP_METHOD(Stub_Issue2705, staticProbe);
PHP_METHOD(Stub_Issue2705, braceNameProbe);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2705_unsetbraceliteral, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2705_unsetbracename, 0, 1, IS_VOID, 0)

	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2705_unsetbracenameandkey, 0, 2, IS_VOID, 0)

	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2705_unsetbracenamelongkey, 0, 1, IS_VOID, 0)

	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2705_unsetnested, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2705_unsetnestedbyvar, 0, 2, IS_VOID, 0)

	ZEND_ARG_INFO(0, outer)
	ZEND_ARG_INFO(0, inner)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2705_unsetdeep, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2705_unsetbraceliteralnested, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2705_unsetbracenamenested, 0, 1, IS_VOID, 0)

	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2705_unsetnestedthenassign, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2705_unsetpropertyoffsetthenassign, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2705_unsetlocaloffsetthenassign, 0, 1, IS_VOID, 0)

	ZEND_ARG_INFO(0, local)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2705_unsetpropertythenassign, 0, 1, IS_VOID, 0)

	ZEND_ARG_INFO(0, target)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2705_unsetnamedpropertythenassign, 0, 2, IS_VOID, 0)

	ZEND_ARG_INFO(0, target)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2705_unsetlocalnested, 0, 0, 1)
	ZEND_ARG_INFO(0, container)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2705_unsetlocaldeep, 0, 0, 1)
	ZEND_ARG_INFO(0, container)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2705_unsetstatic, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2705_unsetstaticnested, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2705_snapshotdefaults, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2705_snapshotstaticdefaults, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2705_nestedprobe, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, iterations, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2705_staticprobe, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, iterations, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2705_bracenameprobe, 0, 2, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, iterations, IS_LONG, 0)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2705_zephir_init_static_properties_stub_issue2705, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2705_method_entry) {
	PHP_ME(Stub_Issue2705, unsetBraceLiteral, arginfo_stub_issue2705_unsetbraceliteral, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2705, unsetBraceName, arginfo_stub_issue2705_unsetbracename, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2705, unsetBraceNameAndKey, arginfo_stub_issue2705_unsetbracenameandkey, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2705, unsetBraceNameLongKey, arginfo_stub_issue2705_unsetbracenamelongkey, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2705, unsetNested, arginfo_stub_issue2705_unsetnested, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2705, unsetNestedByVar, arginfo_stub_issue2705_unsetnestedbyvar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2705, unsetDeep, arginfo_stub_issue2705_unsetdeep, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2705, unsetBraceLiteralNested, arginfo_stub_issue2705_unsetbraceliteralnested, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2705, unsetBraceNameNested, arginfo_stub_issue2705_unsetbracenamenested, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2705, unsetNestedThenAssign, arginfo_stub_issue2705_unsetnestedthenassign, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2705, unsetPropertyOffsetThenAssign, arginfo_stub_issue2705_unsetpropertyoffsetthenassign, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2705, unsetLocalOffsetThenAssign, arginfo_stub_issue2705_unsetlocaloffsetthenassign, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2705, unsetPropertyThenAssign, arginfo_stub_issue2705_unsetpropertythenassign, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2705, unsetNamedPropertyThenAssign, arginfo_stub_issue2705_unsetnamedpropertythenassign, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2705, unsetLocalNested, arginfo_stub_issue2705_unsetlocalnested, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2705, unsetLocalDeep, arginfo_stub_issue2705_unsetlocaldeep, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2705, unsetStatic, arginfo_stub_issue2705_unsetstatic, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2705, unsetStaticNested, arginfo_stub_issue2705_unsetstaticnested, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2705, snapshotDefaults, arginfo_stub_issue2705_snapshotdefaults, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2705, snapshotStaticDefaults, arginfo_stub_issue2705_snapshotstaticdefaults, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2705, nestedProbe, arginfo_stub_issue2705_nestedprobe, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2705, staticProbe, arginfo_stub_issue2705_staticprobe, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2705, braceNameProbe, arginfo_stub_issue2705_bracenameprobe, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
