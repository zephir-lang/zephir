
extern zend_class_entry *stub_issue2698_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2698);

PHP_METHOD(Stub_Issue2698, writeOffset);
PHP_METHOD(Stub_Issue2698, appendValue);
PHP_METHOD(Stub_Issue2698, writeNested);
PHP_METHOD(Stub_Issue2698, removeOffset);
PHP_METHOD(Stub_Issue2698, getItems);
PHP_METHOD(Stub_Issue2698, getBag);
PHP_METHOD(Stub_Issue2698, getMatrix);
PHP_METHOD(Stub_Issue2698, getRemovable);
PHP_METHOD(Stub_Issue2698, writeProbe);
PHP_METHOD(Stub_Issue2698, appendProbe);
PHP_METHOD(Stub_Issue2698, nestedProbe);
PHP_METHOD(Stub_Issue2698, unsetProbe);
PHP_METHOD(Stub_Issue2698, unsetSharedProbe);
PHP_METHOD(Stub_Issue2698, leakProbeControl);
zend_object *zephir_init_properties_Stub_Issue2698(zend_class_entry *class_type);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2698_writeoffset, 0, 2, IS_VOID, 0)

	ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2698_appendvalue, 0, 1, IS_VOID, 0)

	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2698_writenested, 0, 3, IS_VOID, 0)

	ZEND_ARG_TYPE_INFO(0, outer, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, inner, IS_STRING, 0)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2698_removeoffset, 0, 1, IS_VOID, 0)

	ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2698_getitems, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2698_getbag, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2698_getmatrix, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2698_getremovable, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2698_writeprobe, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, iterations, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2698_appendprobe, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, iterations, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2698_nestedprobe, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, iterations, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2698_unsetprobe, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, iterations, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2698_unsetsharedprobe, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, iterations, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2698_leakprobecontrol, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, iterations, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2698_zephir_init_properties_stub_issue2698, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2698_method_entry) {
	PHP_ME(Stub_Issue2698, writeOffset, arginfo_stub_issue2698_writeoffset, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2698, appendValue, arginfo_stub_issue2698_appendvalue, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2698, writeNested, arginfo_stub_issue2698_writenested, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2698, removeOffset, arginfo_stub_issue2698_removeoffset, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2698, getItems, arginfo_stub_issue2698_getitems, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2698, getBag, arginfo_stub_issue2698_getbag, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2698, getMatrix, arginfo_stub_issue2698_getmatrix, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2698, getRemovable, arginfo_stub_issue2698_getremovable, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2698, writeProbe, arginfo_stub_issue2698_writeprobe, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2698, appendProbe, arginfo_stub_issue2698_appendprobe, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2698, nestedProbe, arginfo_stub_issue2698_nestedprobe, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2698, unsetProbe, arginfo_stub_issue2698_unsetprobe, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2698, unsetSharedProbe, arginfo_stub_issue2698_unsetsharedprobe, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2698, leakProbeControl, arginfo_stub_issue2698_leakprobecontrol, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
