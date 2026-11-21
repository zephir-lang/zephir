
extern zend_class_entry *stub_issue2825_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2825);

PHP_METHOD(Stub_Issue2825, imported);
PHP_METHOD(Stub_Issue2825, aliased);
PHP_METHOD(Stub_Issue2825, aliasInOtherCase);
PHP_METHOD(Stub_Issue2825, qualified);
PHP_METHOD(Stub_Issue2825, importedWithBackslash);
PHP_METHOD(Stub_Issue2825, keptAsWritten);
PHP_METHOD(Stub_Issue2825, unknown);
PHP_METHOD(Stub_Issue2825, unknownQualified);
PHP_METHOD(Stub_Issue2825, concatenated);
PHP_METHOD(Stub_Issue2825, parentName);
PHP_METHOD(Stub_Issue2825, newAliasInOtherCase);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2825_imported, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2825_aliased, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2825_aliasinothercase, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2825_qualified, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2825_importedwithbackslash, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2825_keptaswritten, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2825_unknown, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2825_unknownqualified, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2825_concatenated, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2825_parentname, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2825_newaliasinothercase, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2825_method_entry) {
	PHP_ME(Stub_Issue2825, imported, arginfo_stub_issue2825_imported, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2825, aliased, arginfo_stub_issue2825_aliased, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2825, aliasInOtherCase, arginfo_stub_issue2825_aliasinothercase, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2825, qualified, arginfo_stub_issue2825_qualified, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2825, importedWithBackslash, arginfo_stub_issue2825_importedwithbackslash, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2825, keptAsWritten, arginfo_stub_issue2825_keptaswritten, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2825, unknown, arginfo_stub_issue2825_unknown, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2825, unknownQualified, arginfo_stub_issue2825_unknownqualified, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2825, concatenated, arginfo_stub_issue2825_concatenated, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2825, parentName, arginfo_stub_issue2825_parentname, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Issue2825, newAliasInOtherCase, arginfo_stub_issue2825_newaliasinothercase, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
