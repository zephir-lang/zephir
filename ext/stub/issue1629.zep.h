
extern zend_class_entry *stub_issue1629_ce;

ZEPHIR_INIT_CLASS(Stub_Issue1629);

PHP_METHOD(Stub_Issue1629, stringTarget);
PHP_METHOD(Stub_Issue1629, charTarget);
PHP_METHOD(Stub_Issue1629, rebuild);
PHP_METHOD(Stub_Issue1629, varTarget);
PHP_METHOD(Stub_Issue1629, intTarget);
PHP_METHOD(Stub_Issue1629, arrayIndexTarget);
PHP_METHOD(Stub_Issue1629, propertyAppendTarget);
PHP_METHOD(Stub_Issue1629, propertyIndexTarget);
PHP_METHOD(Stub_Issue1629, staticIndexTarget);
PHP_METHOD(Stub_Issue1629, staticAppendTarget);
PHP_METHOD(Stub_Issue1629, charToStringTarget);
PHP_METHOD(Stub_Issue1629, intToStringTarget);
PHP_METHOD(Stub_Issue1629, castStringOfChar);
PHP_METHOD(Stub_Issue1629, castStringOfOffset);
PHP_METHOD(Stub_Issue1629, castCharOfVar);
PHP_METHOD(Stub_Issue1629, castUcharOfVar);
PHP_METHOD(Stub_Issue1629, ucharParam);
PHP_METHOD(Stub_Issue1629, keyedCharArray);
PHP_METHOD(Stub_Issue1629, doubleFromChar);
PHP_METHOD(Stub_Issue1629, boolFromChar);
PHP_METHOD(Stub_Issue1629, dynamicFromCharCast);
PHP_METHOD(Stub_Issue1629, returnCharLiteral);
PHP_METHOD(Stub_Issue1629, appendCharLiteral);
zend_object *zephir_init_properties_Stub_Issue1629(zend_class_entry *class_type);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue1629_stringtarget, 0, 2, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, keyspace, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, nb, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue1629_chartarget, 0, 2, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, keyspace, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, nb, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue1629_rebuild, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, keyspace, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue1629_vartarget, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, keyspace, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, nb, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue1629_inttarget, 0, 2, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, keyspace, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, nb, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue1629_arrayindextarget, 0, 2, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, keyspace, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, nb, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue1629_propertyappendtarget, 0, 2, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, keyspace, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, nb, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue1629_propertyindextarget, 0, 2, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, keyspace, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, nb, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue1629_staticindextarget, 0, 2, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, keyspace, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, nb, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue1629_staticappendtarget, 0, 2, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, keyspace, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, nb, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue1629_chartostringtarget, 0, 2, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, keyspace, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, nb, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue1629_inttostringtarget, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue1629_caststringofchar, 0, 2, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, keyspace, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, nb, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue1629_caststringofoffset, 0, 2, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, keyspace, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, nb, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue1629_castcharofvar, 0, 1, IS_LONG, 0)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue1629_castucharofvar, 0, 1, IS_LONG, 0)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue1629_ucharparam, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, value, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue1629_keyedchararray, 0, 2, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, keyspace, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, nb, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue1629_doublefromchar, 0, 2, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, keyspace, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, nb, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue1629_boolfromchar, 0, 2, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, keyspace, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, nb, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue1629_dynamicfromcharcast, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue1629_returncharliteral, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue1629_appendcharliteral, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue1629_zephir_init_properties_stub_issue1629, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue1629_zephir_init_static_properties_stub_issue1629, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue1629_method_entry) {
	PHP_ME(Stub_Issue1629, stringTarget, arginfo_stub_issue1629_stringtarget, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue1629, charTarget, arginfo_stub_issue1629_chartarget, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue1629, rebuild, arginfo_stub_issue1629_rebuild, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue1629, varTarget, arginfo_stub_issue1629_vartarget, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue1629, intTarget, arginfo_stub_issue1629_inttarget, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue1629, arrayIndexTarget, arginfo_stub_issue1629_arrayindextarget, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue1629, propertyAppendTarget, arginfo_stub_issue1629_propertyappendtarget, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue1629, propertyIndexTarget, arginfo_stub_issue1629_propertyindextarget, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue1629, staticIndexTarget, arginfo_stub_issue1629_staticindextarget, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue1629, staticAppendTarget, arginfo_stub_issue1629_staticappendtarget, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue1629, charToStringTarget, arginfo_stub_issue1629_chartostringtarget, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue1629, intToStringTarget, arginfo_stub_issue1629_inttostringtarget, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue1629, castStringOfChar, arginfo_stub_issue1629_caststringofchar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue1629, castStringOfOffset, arginfo_stub_issue1629_caststringofoffset, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue1629, castCharOfVar, arginfo_stub_issue1629_castcharofvar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue1629, castUcharOfVar, arginfo_stub_issue1629_castucharofvar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue1629, ucharParam, arginfo_stub_issue1629_ucharparam, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue1629, keyedCharArray, arginfo_stub_issue1629_keyedchararray, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue1629, doubleFromChar, arginfo_stub_issue1629_doublefromchar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue1629, boolFromChar, arginfo_stub_issue1629_boolfromchar, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Issue1629, dynamicFromCharCast, arginfo_stub_issue1629_dynamicfromcharcast, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue1629, returnCharLiteral, arginfo_stub_issue1629_returncharliteral, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue1629, appendCharLiteral, arginfo_stub_issue1629_appendcharliteral, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
