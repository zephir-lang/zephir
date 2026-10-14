
extern zend_class_entry *stub_issue2391_accessors_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2391_Accessors);

PHP_METHOD(Stub_Issue2391_Accessors, getFlag);
PHP_METHOD(Stub_Issue2391_Accessors, getThing);
PHP_METHOD(Stub_Issue2391_Accessors, getObj);
PHP_METHOD(Stub_Issue2391_Accessors, getMaybe);
PHP_METHOD(Stub_Issue2391_Accessors, getKey);
PHP_METHOD(Stub_Issue2391_Accessors, getListOrFalse);
PHP_METHOD(Stub_Issue2391_Accessors, getRatio);
PHP_METHOD(Stub_Issue2391_Accessors, getName);
PHP_METHOD(Stub_Issue2391_Accessors, __toString);
PHP_METHOD(Stub_Issue2391_Accessors, poke);
PHP_METHOD(Stub_Issue2391_Accessors, flagByHand);
PHP_METHOD(Stub_Issue2391_Accessors, thingByHand);
PHP_METHOD(Stub_Issue2391_Accessors, maybeByHand);
PHP_METHOD(Stub_Issue2391_Accessors, objByHand);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2391_accessors_getflag, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_issue2391_accessors_getthing, 0, 0, Stub\\Issue2391\\Collaborator, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_stub_issue2391_accessors_getobj, 0, 0, MAY_BE_OBJECT)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_issue2391_accessors_getmaybe, 0, 0, Stub\\Issue2391\\Collaborator, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_stub_issue2391_accessors_getkey, 0, 0, MAY_BE_LONG|MAY_BE_STRING)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_stub_issue2391_accessors_getlistorfalse, 0, 0, MAY_BE_ARRAY|MAY_BE_FALSE)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2391_accessors_getratio, 0, 0, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2391_accessors_getname, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2391_accessors___tostring, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_issue2391_accessors_poke, 0, 2, Stub\\Issue2391\\Accessors, 0)
	ZEND_ARG_TYPE_INFO(0, property, IS_STRING, 0)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2391_accessors_flagbyhand, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_issue2391_accessors_thingbyhand, 0, 0, Stub\\Issue2391\\Collaborator, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_issue2391_accessors_maybebyhand, 0, 0, Stub\\Issue2391\\Collaborator, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_stub_issue2391_accessors_objbyhand, 0, 0, MAY_BE_OBJECT)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2391_accessors_method_entry) {
	PHP_ME(Stub_Issue2391_Accessors, getFlag, arginfo_stub_issue2391_accessors_getflag, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2391_Accessors, getThing, arginfo_stub_issue2391_accessors_getthing, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2391_Accessors, getObj, arginfo_stub_issue2391_accessors_getobj, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2391_Accessors, getMaybe, arginfo_stub_issue2391_accessors_getmaybe, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2391_Accessors, getKey, arginfo_stub_issue2391_accessors_getkey, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2391_Accessors, getListOrFalse, arginfo_stub_issue2391_accessors_getlistorfalse, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2391_Accessors, getRatio, arginfo_stub_issue2391_accessors_getratio, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2391_Accessors, getName, arginfo_stub_issue2391_accessors_getname, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2391_Accessors, __toString, arginfo_stub_issue2391_accessors___tostring, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2391_Accessors, poke, arginfo_stub_issue2391_accessors_poke, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2391_Accessors, flagByHand, arginfo_stub_issue2391_accessors_flagbyhand, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2391_Accessors, thingByHand, arginfo_stub_issue2391_accessors_thingbyhand, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2391_Accessors, maybeByHand, arginfo_stub_issue2391_accessors_maybebyhand, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2391_Accessors, objByHand, arginfo_stub_issue2391_accessors_objbyhand, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
