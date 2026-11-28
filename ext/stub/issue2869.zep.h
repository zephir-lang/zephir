
extern zend_class_entry *stub_issue2869_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2869);

PHP_METHOD(Stub_Issue2869, withDefault);
PHP_METHOD(Stub_Issue2869, grouped);
PHP_METHOD(Stub_Issue2869, inverted);
PHP_METHOD(Stub_Issue2869, unique);
PHP_METHOD(Stub_Issue2869, isGroup);
PHP_METHOD(Stub_Issue2869, symlinks);
PHP_METHOD(Stub_Issue2869, assoc);
PHP_METHOD(Stub_Issue2869, props);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2869_withdefault, 0, 0, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, flags, IS_LONG, 0, "\\PDO::FETCH_GROUP")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2869_grouped, 0, 1, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, flags, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2869_inverted, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2869_unique, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2869_isgroup, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, mode, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2869_symlinks, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2869_assoc, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2869_props, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2869_method_entry) {
	PHP_ME(Stub_Issue2869, withDefault, arginfo_stub_issue2869_withdefault, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2869, grouped, arginfo_stub_issue2869_grouped, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2869, inverted, arginfo_stub_issue2869_inverted, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2869, unique, arginfo_stub_issue2869_unique, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2869, isGroup, arginfo_stub_issue2869_isgroup, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2869, symlinks, arginfo_stub_issue2869_symlinks, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2869, assoc, arginfo_stub_issue2869_assoc, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
PHP_ME(Stub_Issue2869, props, arginfo_stub_issue2869_props, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
