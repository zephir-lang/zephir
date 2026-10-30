
extern zend_class_entry *stub_issue2747_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2747);

PHP_METHOD(Stub_Issue2747, add);
PHP_METHOD(Stub_Issue2747, sub);
PHP_METHOD(Stub_Issue2747, mul);
PHP_METHOD(Stub_Issue2747, div);
PHP_METHOD(Stub_Issue2747, mod);
PHP_METHOD(Stub_Issue2747, concat);
PHP_METHOD(Stub_Issue2747, bitwiseAnd);
PHP_METHOD(Stub_Issue2747, bitwiseOr);
PHP_METHOD(Stub_Issue2747, bitwiseXor);
PHP_METHOD(Stub_Issue2747, shiftLeft);
PHP_METHOD(Stub_Issue2747, shiftRight);
PHP_METHOD(Stub_Issue2747, addStringLiteralKey);
PHP_METHOD(Stub_Issue2747, addIntLiteralKey);
PHP_METHOD(Stub_Issue2747, addLongKey);
PHP_METHOD(Stub_Issue2747, addStringKey);
PHP_METHOD(Stub_Issue2747, addIntLiteral);
PHP_METHOD(Stub_Issue2747, concatStringLiteral);
PHP_METHOD(Stub_Issue2747, addLongValue);
PHP_METHOD(Stub_Issue2747, mulDoubleValue);
PHP_METHOD(Stub_Issue2747, concatOwnElement);
PHP_METHOD(Stub_Issue2747, addToCopy);
PHP_METHOD(Stub_Issue2747, addNested);
PHP_METHOD(Stub_Issue2747, concatNestedLiteral);
PHP_METHOD(Stub_Issue2747, subAppend);
PHP_METHOD(Stub_Issue2747, concatNestedAppend);
PHP_METHOD(Stub_Issue2747, addThis);
PHP_METHOD(Stub_Issue2747, addThisNested);
PHP_METHOD(Stub_Issue2747, subThisAppend);
PHP_METHOD(Stub_Issue2747, concatThisNestedAppend);
PHP_METHOD(Stub_Issue2747, addObject);
PHP_METHOD(Stub_Issue2747, addStatic);
PHP_METHOD(Stub_Issue2747, addStaticNested);
PHP_METHOD(Stub_Issue2747, subStaticAppend);
PHP_METHOD(Stub_Issue2747, concatStaticNestedAppend);
PHP_METHOD(Stub_Issue2747, setProperties);
PHP_METHOD(Stub_Issue2747, key);
PHP_METHOD(Stub_Issue2747, value);
PHP_METHOD(Stub_Issue2747, bump);
PHP_METHOD(Stub_Issue2747, orderLocal);
PHP_METHOD(Stub_Issue2747, orderLocalNested);
PHP_METHOD(Stub_Issue2747, orderLocalAppend);
PHP_METHOD(Stub_Issue2747, orderCompound);
PHP_METHOD(Stub_Issue2747, orderThis);
PHP_METHOD(Stub_Issue2747, orderThisNested);
PHP_METHOD(Stub_Issue2747, orderStatic);
PHP_METHOD(Stub_Issue2747, orderStaticAppend);
PHP_METHOD(Stub_Issue2747, orderPropertyIndex);
PHP_METHOD(Stub_Issue2747, orderPropertyValue);
PHP_METHOD(Stub_Issue2747, orderComputedIndex);
PHP_METHOD(Stub_Issue2747, orderStringOffset);
PHP_METHOD(Stub_Issue2747, orderVariableIndex);
PHP_METHOD(Stub_Issue2747, orderVariableExpressionIndex);
PHP_METHOD(Stub_Issue2747, orderLiteralIndex);
zend_object *zephir_init_properties_Stub_Issue2747(zend_class_entry *class_type);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2747_add, 0, 0, 3)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, k)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2747_sub, 0, 0, 3)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, k)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2747_mul, 0, 0, 3)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, k)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2747_div, 0, 0, 3)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, k)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2747_mod, 0, 0, 3)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, k)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2747_concat, 0, 0, 3)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, k)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2747_bitwiseand, 0, 0, 3)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, k)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2747_bitwiseor, 0, 0, 3)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, k)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2747_bitwisexor, 0, 0, 3)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, k)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2747_shiftleft, 0, 0, 3)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, k)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2747_shiftright, 0, 0, 3)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, k)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2747_addstringliteralkey, 0, 0, 2)
	ZEND_ARG_ARRAY_INFO(0, a, 0)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2747_addintliteralkey, 0, 0, 2)
	ZEND_ARG_ARRAY_INFO(0, a, 0)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2747_addlongkey, 0, 0, 3)
	ZEND_ARG_ARRAY_INFO(0, a, 0)
	ZEND_ARG_TYPE_INFO(0, k, IS_LONG, 0)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2747_addstringkey, 0, 0, 3)
	ZEND_ARG_ARRAY_INFO(0, a, 0)
	ZEND_ARG_TYPE_INFO(0, k, IS_STRING, 0)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2747_addintliteral, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, a, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2747_concatstringliteral, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, a, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2747_addlongvalue, 0, 0, 3)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, k)
	ZEND_ARG_TYPE_INFO(0, v, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2747_muldoublevalue, 0, 0, 3)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, k)
	ZEND_ARG_TYPE_INFO(0, v, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2747_concatownelement, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, a, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2747_addtocopy, 0, 3, IS_ARRAY, 0)
	ZEND_ARG_ARRAY_INFO(0, a, 0)
	ZEND_ARG_INFO(0, k)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2747_addnested, 0, 0, 4)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, i)
	ZEND_ARG_INFO(0, j)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2747_concatnestedliteral, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2747_subappend, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2747_concatnestedappend, 0, 0, 3)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, k)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2747_addthis, 0, 0, 2)
	ZEND_ARG_INFO(0, k)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2747_addthisnested, 0, 0, 3)
	ZEND_ARG_INFO(0, k)
	ZEND_ARG_INFO(0, j)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2747_subthisappend, 0, 0, 1)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2747_concatthisnestedappend, 0, 0, 2)
	ZEND_ARG_INFO(0, k)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2747_addobject, 0, 0, 3)
	ZEND_ARG_INFO(0, o)
	ZEND_ARG_INFO(0, k)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2747_addstatic, 0, 0, 2)
	ZEND_ARG_INFO(0, k)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2747_addstaticnested, 0, 0, 3)
	ZEND_ARG_INFO(0, k)
	ZEND_ARG_INFO(0, j)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2747_substaticappend, 0, 0, 1)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2747_concatstaticnestedappend, 0, 0, 2)
	ZEND_ARG_INFO(0, k)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2747_setproperties, 0, 1, IS_VOID, 0)

	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2747_key, 0, 0, 1)
	ZEND_ARG_INFO(0, n)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2747_value, 0, 0, 1)
	ZEND_ARG_INFO(0, n)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2747_bump, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2747_orderlocal, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2747_orderlocalnested, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2747_orderlocalappend, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2747_ordercompound, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2747_orderthis, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2747_orderthisnested, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2747_orderstatic, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2747_orderstaticappend, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2747_orderpropertyindex, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2747_orderpropertyvalue, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2747_ordercomputedindex, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2747_orderstringoffset, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2747_ordervariableindex, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2747_ordervariableexpressionindex, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2747_orderliteralindex, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2747_zephir_init_properties_stub_issue2747, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2747_zephir_init_static_properties_stub_issue2747, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2747_method_entry) {
	PHP_ME(Stub_Issue2747, add, arginfo_stub_issue2747_add, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2747, sub, arginfo_stub_issue2747_sub, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2747, mul, arginfo_stub_issue2747_mul, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2747, div, arginfo_stub_issue2747_div, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2747, mod, arginfo_stub_issue2747_mod, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2747, concat, arginfo_stub_issue2747_concat, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2747, bitwiseAnd, arginfo_stub_issue2747_bitwiseand, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2747, bitwiseOr, arginfo_stub_issue2747_bitwiseor, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2747, bitwiseXor, arginfo_stub_issue2747_bitwisexor, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2747, shiftLeft, arginfo_stub_issue2747_shiftleft, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2747, shiftRight, arginfo_stub_issue2747_shiftright, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2747, addStringLiteralKey, arginfo_stub_issue2747_addstringliteralkey, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2747, addIntLiteralKey, arginfo_stub_issue2747_addintliteralkey, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2747, addLongKey, arginfo_stub_issue2747_addlongkey, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2747, addStringKey, arginfo_stub_issue2747_addstringkey, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2747, addIntLiteral, arginfo_stub_issue2747_addintliteral, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2747, concatStringLiteral, arginfo_stub_issue2747_concatstringliteral, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2747, addLongValue, arginfo_stub_issue2747_addlongvalue, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2747, mulDoubleValue, arginfo_stub_issue2747_muldoublevalue, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2747, concatOwnElement, arginfo_stub_issue2747_concatownelement, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2747, addToCopy, arginfo_stub_issue2747_addtocopy, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2747, addNested, arginfo_stub_issue2747_addnested, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2747, concatNestedLiteral, arginfo_stub_issue2747_concatnestedliteral, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2747, subAppend, arginfo_stub_issue2747_subappend, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2747, concatNestedAppend, arginfo_stub_issue2747_concatnestedappend, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2747, addThis, arginfo_stub_issue2747_addthis, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2747, addThisNested, arginfo_stub_issue2747_addthisnested, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2747, subThisAppend, arginfo_stub_issue2747_subthisappend, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2747, concatThisNestedAppend, arginfo_stub_issue2747_concatthisnestedappend, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2747, addObject, arginfo_stub_issue2747_addobject, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2747, addStatic, arginfo_stub_issue2747_addstatic, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2747, addStaticNested, arginfo_stub_issue2747_addstaticnested, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2747, subStaticAppend, arginfo_stub_issue2747_substaticappend, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2747, concatStaticNestedAppend, arginfo_stub_issue2747_concatstaticnestedappend, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2747, setProperties, arginfo_stub_issue2747_setproperties, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2747, key, arginfo_stub_issue2747_key, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2747, value, arginfo_stub_issue2747_value, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Issue2747, bump, arginfo_stub_issue2747_bump, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2747, orderLocal, arginfo_stub_issue2747_orderlocal, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2747, orderLocalNested, arginfo_stub_issue2747_orderlocalnested, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2747, orderLocalAppend, arginfo_stub_issue2747_orderlocalappend, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2747, orderCompound, arginfo_stub_issue2747_ordercompound, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2747, orderThis, arginfo_stub_issue2747_orderthis, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2747, orderThisNested, arginfo_stub_issue2747_orderthisnested, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2747, orderStatic, arginfo_stub_issue2747_orderstatic, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2747, orderStaticAppend, arginfo_stub_issue2747_orderstaticappend, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2747, orderPropertyIndex, arginfo_stub_issue2747_orderpropertyindex, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2747, orderPropertyValue, arginfo_stub_issue2747_orderpropertyvalue, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2747, orderComputedIndex, arginfo_stub_issue2747_ordercomputedindex, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2747, orderStringOffset, arginfo_stub_issue2747_orderstringoffset, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2747, orderVariableIndex, arginfo_stub_issue2747_ordervariableindex, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2747, orderVariableExpressionIndex, arginfo_stub_issue2747_ordervariableexpressionindex, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2747, orderLiteralIndex, arginfo_stub_issue2747_orderliteralindex, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
