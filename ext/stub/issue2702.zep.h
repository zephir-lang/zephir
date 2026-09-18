
extern zend_class_entry *stub_issue2702_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2702);

PHP_METHOD(Stub_Issue2702, getData);
PHP_METHOD(Stub_Issue2702, getRemovable);
PHP_METHOD(Stub_Issue2702, removeLiteral);
PHP_METHOD(Stub_Issue2702, removeLongLiteral);
PHP_METHOD(Stub_Issue2702, removeByVar);
PHP_METHOD(Stub_Issue2702, removeByNativeString);
PHP_METHOD(Stub_Issue2702, removeByNativeInt);
PHP_METHOD(Stub_Issue2702, removeFromTypedProperty);
PHP_METHOD(Stub_Issue2702, removeFromShared);
PHP_METHOD(Stub_Issue2702, unsetContainerLiteral);
PHP_METHOD(Stub_Issue2702, unsetContainerLongLiteral);
PHP_METHOD(Stub_Issue2702, unsetContainerByVar);
PHP_METHOD(Stub_Issue2702, unsetLocalLiteral);
PHP_METHOD(Stub_Issue2702, unsetLocalLongLiteral);
PHP_METHOD(Stub_Issue2702, unsetLocalByVar);
PHP_METHOD(Stub_Issue2702, snapshotAroundLiteralUnset);
PHP_METHOD(Stub_Issue2702, literalProbe);
PHP_METHOD(Stub_Issue2702, longLiteralProbe);
PHP_METHOD(Stub_Issue2702, localArrayProbe);
PHP_METHOD(Stub_Issue2702, growthProbe);
zend_object *zephir_init_properties_Stub_Issue2702(zend_class_entry *class_type);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2702_getdata, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2702_getremovable, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2702_removeliteral, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2702_removelongliteral, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2702_removebyvar, 0, 1, IS_VOID, 0)

	ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2702_removebynativestring, 0, 1, IS_VOID, 0)

	ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2702_removebynativeint, 0, 1, IS_VOID, 0)

	ZEND_ARG_TYPE_INFO(0, key, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2702_removefromtypedproperty, 0, 1, IS_VOID, 0)

	ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2702_removefromshared, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2702_unsetcontainerliteral, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2702_unsetcontainerlongliteral, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2702_unsetcontainerbyvar, 0, 1, IS_VOID, 0)

	ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2702_unsetlocalliteral, 0, 0, 1)
	ZEND_ARG_INFO(0, container)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2702_unsetlocallongliteral, 0, 0, 1)
	ZEND_ARG_INFO(0, container)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2702_unsetlocalbyvar, 0, 0, 2)
	ZEND_ARG_INFO(0, container)
	ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2702_snapshotaroundliteralunset, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2702_literalprobe, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, iterations, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2702_longliteralprobe, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, iterations, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2702_localarrayprobe, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, iterations, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2702_growthprobe, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, iterations, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2702_zephir_init_properties_stub_issue2702, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2702_method_entry) {
	PHP_ME(Stub_Issue2702, getData, arginfo_stub_issue2702_getdata, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2702, getRemovable, arginfo_stub_issue2702_getremovable, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2702, removeLiteral, arginfo_stub_issue2702_removeliteral, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2702, removeLongLiteral, arginfo_stub_issue2702_removelongliteral, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2702, removeByVar, arginfo_stub_issue2702_removebyvar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2702, removeByNativeString, arginfo_stub_issue2702_removebynativestring, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2702, removeByNativeInt, arginfo_stub_issue2702_removebynativeint, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2702, removeFromTypedProperty, arginfo_stub_issue2702_removefromtypedproperty, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2702, removeFromShared, arginfo_stub_issue2702_removefromshared, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2702, unsetContainerLiteral, arginfo_stub_issue2702_unsetcontainerliteral, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2702, unsetContainerLongLiteral, arginfo_stub_issue2702_unsetcontainerlongliteral, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2702, unsetContainerByVar, arginfo_stub_issue2702_unsetcontainerbyvar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2702, unsetLocalLiteral, arginfo_stub_issue2702_unsetlocalliteral, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2702, unsetLocalLongLiteral, arginfo_stub_issue2702_unsetlocallongliteral, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2702, unsetLocalByVar, arginfo_stub_issue2702_unsetlocalbyvar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2702, snapshotAroundLiteralUnset, arginfo_stub_issue2702_snapshotaroundliteralunset, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2702, literalProbe, arginfo_stub_issue2702_literalprobe, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2702, longLiteralProbe, arginfo_stub_issue2702_longliteralprobe, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2702, localArrayProbe, arginfo_stub_issue2702_localarrayprobe, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2702, growthProbe, arginfo_stub_issue2702_growthprobe, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
