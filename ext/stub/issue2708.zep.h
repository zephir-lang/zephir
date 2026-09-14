
extern zend_class_entry *stub_issue2708_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2708);

PHP_METHOD(Stub_Issue2708, issetOnConstantLiteral);
PHP_METHOD(Stub_Issue2708, emptyOnConstantLiteral);
PHP_METHOD(Stub_Issue2708, fetchOnConstantLiteral);
PHP_METHOD(Stub_Issue2708, readOnConstantLiteral);
PHP_METHOD(Stub_Issue2708, unsetLiteralOffset);
PHP_METHOD(Stub_Issue2708, writeThenReadBack);
PHP_METHOD(Stub_Issue2708, writeMultiDimensional);
PHP_METHOD(Stub_Issue2708, writeThree);
PHP_METHOD(Stub_Issue2708, writeZero);
PHP_METHOD(Stub_Issue2708, writeNegativeThree);
PHP_METHOD(Stub_Issue2708, writeIntMax);
PHP_METHOD(Stub_Issue2708, writeLeadingZero);
PHP_METHOD(Stub_Issue2708, writeLeadingPlus);
PHP_METHOD(Stub_Issue2708, writeNegativeZero);
PHP_METHOD(Stub_Issue2708, writeLeadingSpace);
PHP_METHOD(Stub_Issue2708, writeTrailingSpace);
PHP_METHOD(Stub_Issue2708, writeDecimalPoint);
PHP_METHOD(Stub_Issue2708, writeOverflow);
PHP_METHOD(Stub_Issue2708, appendAfterNumericStringKey);
PHP_METHOD(Stub_Issue2708, mergeAfterNumericStringKey);
PHP_METHOD(Stub_Issue2708, writeOrdinaryKey);
PHP_METHOD(Stub_Issue2708, readOnContainer);
PHP_METHOD(Stub_Issue2708, issetOnContainer);
PHP_METHOD(Stub_Issue2708, writeOnContainer);
PHP_METHOD(Stub_Issue2708, unsetOnContainer);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2708_issetonconstantliteral, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2708_emptyonconstantliteral, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2708_fetchonconstantliteral, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2708_readonconstantliteral, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2708_unsetliteraloffset, 0, 1, IS_ARRAY, 0)
	ZEND_ARG_ARRAY_INFO(0, a, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2708_writethenreadback, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2708_writemultidimensional, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2708_writethree, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2708_writezero, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2708_writenegativethree, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2708_writeintmax, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2708_writeleadingzero, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2708_writeleadingplus, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2708_writenegativezero, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2708_writeleadingspace, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2708_writetrailingspace, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2708_writedecimalpoint, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2708_writeoverflow, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2708_appendafternumericstringkey, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2708_mergeafternumericstringkey, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2708_writeordinarykey, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2708_readoncontainer, 0, 0, 1)
	ZEND_ARG_INFO(0, container)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2708_issetoncontainer, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, container)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2708_writeoncontainer, 0, 1, IS_VOID, 0)

	ZEND_ARG_INFO(0, container)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2708_unsetoncontainer, 0, 1, IS_VOID, 0)

	ZEND_ARG_INFO(0, container)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2708_method_entry) {
	PHP_ME(Stub_Issue2708, issetOnConstantLiteral, arginfo_stub_issue2708_issetonconstantliteral, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2708, emptyOnConstantLiteral, arginfo_stub_issue2708_emptyonconstantliteral, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Issue2708, fetchOnConstantLiteral, arginfo_stub_issue2708_fetchonconstantliteral, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Issue2708, readOnConstantLiteral, arginfo_stub_issue2708_readonconstantliteral, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2708, unsetLiteralOffset, arginfo_stub_issue2708_unsetliteraloffset, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Issue2708, writeThenReadBack, arginfo_stub_issue2708_writethenreadback, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2708, writeMultiDimensional, arginfo_stub_issue2708_writemultidimensional, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2708, writeThree, arginfo_stub_issue2708_writethree, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2708, writeZero, arginfo_stub_issue2708_writezero, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2708, writeNegativeThree, arginfo_stub_issue2708_writenegativethree, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2708, writeIntMax, arginfo_stub_issue2708_writeintmax, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2708, writeLeadingZero, arginfo_stub_issue2708_writeleadingzero, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2708, writeLeadingPlus, arginfo_stub_issue2708_writeleadingplus, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2708, writeNegativeZero, arginfo_stub_issue2708_writenegativezero, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2708, writeLeadingSpace, arginfo_stub_issue2708_writeleadingspace, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2708, writeTrailingSpace, arginfo_stub_issue2708_writetrailingspace, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2708, writeDecimalPoint, arginfo_stub_issue2708_writedecimalpoint, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2708, writeOverflow, arginfo_stub_issue2708_writeoverflow, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2708, appendAfterNumericStringKey, arginfo_stub_issue2708_appendafternumericstringkey, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2708, mergeAfterNumericStringKey, arginfo_stub_issue2708_mergeafternumericstringkey, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2708, writeOrdinaryKey, arginfo_stub_issue2708_writeordinarykey, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2708, readOnContainer, arginfo_stub_issue2708_readoncontainer, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2708, issetOnContainer, arginfo_stub_issue2708_issetoncontainer, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2708, writeOnContainer, arginfo_stub_issue2708_writeoncontainer, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2708, unsetOnContainer, arginfo_stub_issue2708_unsetoncontainer, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
