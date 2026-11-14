
extern zend_class_entry *stub_issue2808_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2808);

PHP_METHOD(Stub_Issue2808, add);
PHP_METHOD(Stub_Issue2808, sub);
PHP_METHOD(Stub_Issue2808, mul);
PHP_METHOD(Stub_Issue2808, div);
PHP_METHOD(Stub_Issue2808, mod);
PHP_METHOD(Stub_Issue2808, concat);
PHP_METHOD(Stub_Issue2808, bitwiseAnd);
PHP_METHOD(Stub_Issue2808, bitwiseOr);
PHP_METHOD(Stub_Issue2808, bitwiseXor);
PHP_METHOD(Stub_Issue2808, shiftLeft);
PHP_METHOD(Stub_Issue2808, shiftRight);
PHP_METHOD(Stub_Issue2808, addOne);
PHP_METHOD(Stub_Issue2808, addDouble);
PHP_METHOD(Stub_Issue2808, concatLiteral);
PHP_METHOD(Stub_Issue2808, concatChar);
PHP_METHOD(Stub_Issue2808, concatCharLiteral);
PHP_METHOD(Stub_Issue2808, addChar);
PHP_METHOD(Stub_Issue2808, addCall);
PHP_METHOD(Stub_Issue2808, addDynamic);
PHP_METHOD(Stub_Issue2808, concatDynamic);
PHP_METHOD(Stub_Issue2808, addNativeName);
PHP_METHOD(Stub_Issue2808, addString);
PHP_METHOD(Stub_Issue2808, concatString);
PHP_METHOD(Stub_Issue2808, increment);
PHP_METHOD(Stub_Issue2808, decrement);
PHP_METHOD(Stub_Issue2808, assign);
PHP_METHOD(Stub_Issue2808, assignDynamic);
PHP_METHOD(Stub_Issue2808, assignThenMark);
PHP_METHOD(Stub_Issue2808, assignDynamicThenMark);
PHP_METHOD(Stub_Issue2808, addThenMark);
PHP_METHOD(Stub_Issue2808, incrementThenMark);
PHP_METHOD(Stub_Issue2808, identity);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2808_add, 0, 0, 2)
	ZEND_ARG_INFO(0, o)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2808_sub, 0, 0, 2)
	ZEND_ARG_INFO(0, o)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2808_mul, 0, 0, 2)
	ZEND_ARG_INFO(0, o)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2808_div, 0, 0, 2)
	ZEND_ARG_INFO(0, o)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2808_mod, 0, 0, 2)
	ZEND_ARG_INFO(0, o)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2808_concat, 0, 0, 2)
	ZEND_ARG_INFO(0, o)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2808_bitwiseand, 0, 0, 2)
	ZEND_ARG_INFO(0, o)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2808_bitwiseor, 0, 0, 2)
	ZEND_ARG_INFO(0, o)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2808_bitwisexor, 0, 0, 2)
	ZEND_ARG_INFO(0, o)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2808_shiftleft, 0, 0, 2)
	ZEND_ARG_INFO(0, o)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2808_shiftright, 0, 0, 2)
	ZEND_ARG_INFO(0, o)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2808_addone, 0, 0, 1)
	ZEND_ARG_INFO(0, o)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2808_adddouble, 0, 0, 1)
	ZEND_ARG_INFO(0, o)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2808_concatliteral, 0, 0, 1)
	ZEND_ARG_INFO(0, o)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2808_concatchar, 0, 0, 1)
	ZEND_ARG_INFO(0, o)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2808_concatcharliteral, 0, 0, 1)
	ZEND_ARG_INFO(0, o)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2808_addchar, 0, 0, 1)
	ZEND_ARG_INFO(0, o)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2808_addcall, 0, 0, 2)
	ZEND_ARG_INFO(0, o)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2808_adddynamic, 0, 0, 3)
	ZEND_ARG_INFO(0, o)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2808_concatdynamic, 0, 0, 3)
	ZEND_ARG_INFO(0, o)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2808_addnativename, 0, 0, 1)
	ZEND_ARG_INFO(0, o)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2808_addstring, 0, 0, 2)
	ZEND_ARG_INFO(0, o)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2808_concatstring, 0, 0, 2)
	ZEND_ARG_INFO(0, o)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2808_increment, 0, 0, 1)
	ZEND_ARG_INFO(0, o)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2808_decrement, 0, 0, 1)
	ZEND_ARG_INFO(0, o)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2808_assign, 0, 0, 2)
	ZEND_ARG_INFO(0, o)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2808_assigndynamic, 0, 0, 3)
	ZEND_ARG_INFO(0, o)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2808_assignthenmark, 0, 0, 3)
	ZEND_ARG_INFO(0, o)
	ZEND_ARG_INFO(0, v)
	ZEND_ARG_INFO(0, target)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2808_assigndynamicthenmark, 0, 0, 4)
	ZEND_ARG_INFO(0, o)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, v)
	ZEND_ARG_INFO(0, target)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2808_addthenmark, 0, 0, 3)
	ZEND_ARG_INFO(0, o)
	ZEND_ARG_INFO(0, v)
	ZEND_ARG_INFO(0, target)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2808_incrementthenmark, 0, 0, 2)
	ZEND_ARG_INFO(0, o)
	ZEND_ARG_INFO(0, target)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2808_identity, 0, 0, 1)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2808_method_entry) {
	PHP_ME(Stub_Issue2808, add, arginfo_stub_issue2808_add, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2808, sub, arginfo_stub_issue2808_sub, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2808, mul, arginfo_stub_issue2808_mul, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2808, div, arginfo_stub_issue2808_div, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2808, mod, arginfo_stub_issue2808_mod, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2808, concat, arginfo_stub_issue2808_concat, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2808, bitwiseAnd, arginfo_stub_issue2808_bitwiseand, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2808, bitwiseOr, arginfo_stub_issue2808_bitwiseor, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2808, bitwiseXor, arginfo_stub_issue2808_bitwisexor, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2808, shiftLeft, arginfo_stub_issue2808_shiftleft, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2808, shiftRight, arginfo_stub_issue2808_shiftright, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2808, addOne, arginfo_stub_issue2808_addone, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2808, addDouble, arginfo_stub_issue2808_adddouble, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2808, concatLiteral, arginfo_stub_issue2808_concatliteral, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2808, concatChar, arginfo_stub_issue2808_concatchar, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2808, concatCharLiteral, arginfo_stub_issue2808_concatcharliteral, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2808, addChar, arginfo_stub_issue2808_addchar, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2808, addCall, arginfo_stub_issue2808_addcall, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2808, addDynamic, arginfo_stub_issue2808_adddynamic, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2808, concatDynamic, arginfo_stub_issue2808_concatdynamic, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2808, addNativeName, arginfo_stub_issue2808_addnativename, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2808, addString, arginfo_stub_issue2808_addstring, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2808, concatString, arginfo_stub_issue2808_concatstring, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2808, increment, arginfo_stub_issue2808_increment, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2808, decrement, arginfo_stub_issue2808_decrement, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2808, assign, arginfo_stub_issue2808_assign, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2808, assignDynamic, arginfo_stub_issue2808_assigndynamic, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2808, assignThenMark, arginfo_stub_issue2808_assignthenmark, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2808, assignDynamicThenMark, arginfo_stub_issue2808_assigndynamicthenmark, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2808, addThenMark, arginfo_stub_issue2808_addthenmark, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2808, incrementThenMark, arginfo_stub_issue2808_incrementthenmark, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2808, identity, arginfo_stub_issue2808_identity, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
