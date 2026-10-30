
extern zend_class_entry *stub_issue2761_accessors_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2761_Accessors);

PHP_METHOD(Stub_Issue2761_Accessors, getName);
PHP_METHOD(Stub_Issue2761_Accessors, setName);
PHP_METHOD(Stub_Issue2761_Accessors, getThing);
PHP_METHOD(Stub_Issue2761_Accessors, setThing);
PHP_METHOD(Stub_Issue2761_Accessors, getMaybe);
PHP_METHOD(Stub_Issue2761_Accessors, setMaybe);
PHP_METHOD(Stub_Issue2761_Accessors, getCount);
PHP_METHOD(Stub_Issue2761_Accessors, setCount);
PHP_METHOD(Stub_Issue2761_Accessors, getPending);
PHP_METHOD(Stub_Issue2761_Accessors, getDocQualified);
PHP_METHOD(Stub_Issue2761_Accessors, setDocQualified);
PHP_METHOD(Stub_Issue2761_Accessors, getDocRelative);
PHP_METHOD(Stub_Issue2761_Accessors, getDocNullable);
PHP_METHOD(Stub_Issue2761_Accessors, setDocNullable);
PHP_METHOD(Stub_Issue2761_Accessors, getDeclaredWins);
PHP_METHOD(Stub_Issue2761_Accessors, setDeclaredWins);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2761_accessors_getname, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2761_accessors_setname, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_issue2761_accessors_getthing, 0, 0, Stub\\Issue2761\\Collaborator, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2761_accessors_setthing, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, thing, Stub\\Issue2761\\Collaborator, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_issue2761_accessors_getmaybe, 0, 0, Stub\\Issue2761\\Collaborator, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2761_accessors_setmaybe, 0, 0, 1)
	ZEND_ARG_OBJ_TYPE_MASK(0, maybe, Stub\\Issue2761\\Collaborator, MAY_BE_NULL, NULL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2761_accessors_getcount, 0, 0, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2761_accessors_setcount, 0, 0, 1)
	ZEND_ARG_TYPE_MASK(0, count, MAY_BE_LONG|MAY_BE_NULL, NULL)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2761_accessors_getpending, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_issue2761_accessors_getdocqualified, 0, 0, Stub\\Issue2761\\Collaborator, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2761_accessors_setdocqualified, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, docQualified, Stub\\Issue2761\\Collaborator, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_issue2761_accessors_getdocrelative, 0, 0, Stub\\Issue2761\\Collaborator, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_issue2761_accessors_getdocnullable, 0, 0, Stub\\Issue2761\\Collaborator, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2761_accessors_setdocnullable, 0, 0, 1)
	ZEND_ARG_INFO(0, docNullable)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2761_accessors_getdeclaredwins, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2761_accessors_setdeclaredwins, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, declaredWins, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2761_accessors_method_entry) {
	PHP_ME(Stub_Issue2761_Accessors, getName, arginfo_stub_issue2761_accessors_getname, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2761_Accessors, setName, arginfo_stub_issue2761_accessors_setname, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2761_Accessors, getThing, arginfo_stub_issue2761_accessors_getthing, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2761_Accessors, setThing, arginfo_stub_issue2761_accessors_setthing, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2761_Accessors, getMaybe, arginfo_stub_issue2761_accessors_getmaybe, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2761_Accessors, setMaybe, arginfo_stub_issue2761_accessors_setmaybe, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2761_Accessors, getCount, arginfo_stub_issue2761_accessors_getcount, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2761_Accessors, setCount, arginfo_stub_issue2761_accessors_setcount, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2761_Accessors, getPending, arginfo_stub_issue2761_accessors_getpending, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2761_Accessors, getDocQualified, arginfo_stub_issue2761_accessors_getdocqualified, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2761_Accessors, setDocQualified, arginfo_stub_issue2761_accessors_setdocqualified, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2761_Accessors, getDocRelative, arginfo_stub_issue2761_accessors_getdocrelative, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2761_Accessors, getDocNullable, arginfo_stub_issue2761_accessors_getdocnullable, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2761_Accessors, setDocNullable, arginfo_stub_issue2761_accessors_setdocnullable, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2761_Accessors, getDeclaredWins, arginfo_stub_issue2761_accessors_getdeclaredwins, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2761_Accessors, setDeclaredWins, arginfo_stub_issue2761_accessors_setdeclaredwins, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
