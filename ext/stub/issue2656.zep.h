
extern zend_class_entry *stub_issue2656_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2656);

PHP_METHOD(Stub_Issue2656, setContainer);
PHP_METHOD(Stub_Issue2656, fetchLiteral);
PHP_METHOD(Stub_Issue2656, fetchLiteralMissing);
PHP_METHOD(Stub_Issue2656, emptyLiteral);
PHP_METHOD(Stub_Issue2656, issetLiteral);
PHP_METHOD(Stub_Issue2656, fetchLong);
PHP_METHOD(Stub_Issue2656, issetLong);
PHP_METHOD(Stub_Issue2656, fetchDynamic);
PHP_METHOD(Stub_Issue2656, issetDynamic);
PHP_METHOD(Stub_Issue2656, fetchThroughProperty);
PHP_METHOD(Stub_Issue2656, leakProbe);
PHP_METHOD(Stub_Issue2656, leakProbeControl);
PHP_METHOD(Stub_Issue2656, unsetPropertyProbe);
PHP_METHOD(Stub_Issue2656, throwArrayProbe);
zend_object *zephir_init_properties_Stub_Issue2656(zend_class_entry *class_type);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2656_setcontainer, 0, 1, IS_VOID, 0)

	ZEND_ARG_INFO(0, container)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2656_fetchliteral, 0, 0, 1)
	ZEND_ARG_INFO(0, container)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2656_fetchliteralmissing, 0, 0, 1)
	ZEND_ARG_INFO(0, container)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2656_emptyliteral, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, container)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2656_issetliteral, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, container)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2656_fetchlong, 0, 0, 1)
	ZEND_ARG_INFO(0, container)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2656_issetlong, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, container)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2656_fetchdynamic, 0, 0, 2)
	ZEND_ARG_INFO(0, container)
	ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2656_issetdynamic, 0, 2, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, container)
	ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2656_fetchthroughproperty, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2656_leakprobe, 0, 3, IS_LONG, 0)
	ZEND_ARG_INFO(0, container)
	ZEND_ARG_INFO(0, key)
	ZEND_ARG_TYPE_INFO(0, iterations, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2656_leakprobecontrol, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, iterations, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2656_unsetpropertyprobe, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, iterations, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2656_throwarrayprobe, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, iterations, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2656_zephir_init_properties_stub_issue2656, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2656_method_entry) {
	PHP_ME(Stub_Issue2656, setContainer, arginfo_stub_issue2656_setcontainer, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2656, fetchLiteral, arginfo_stub_issue2656_fetchliteral, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2656, fetchLiteralMissing, arginfo_stub_issue2656_fetchliteralmissing, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2656, emptyLiteral, arginfo_stub_issue2656_emptyliteral, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2656, issetLiteral, arginfo_stub_issue2656_issetliteral, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2656, fetchLong, arginfo_stub_issue2656_fetchlong, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2656, issetLong, arginfo_stub_issue2656_issetlong, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2656, fetchDynamic, arginfo_stub_issue2656_fetchdynamic, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2656, issetDynamic, arginfo_stub_issue2656_issetdynamic, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Issue2656, fetchThroughProperty, arginfo_stub_issue2656_fetchthroughproperty, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2656, leakProbe, arginfo_stub_issue2656_leakprobe, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2656, leakProbeControl, arginfo_stub_issue2656_leakprobecontrol, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2656, unsetPropertyProbe, arginfo_stub_issue2656_unsetpropertyprobe, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2656, throwArrayProbe, arginfo_stub_issue2656_throwarrayprobe, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
