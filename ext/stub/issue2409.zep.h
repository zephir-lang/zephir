
extern zend_class_entry *stub_issue2409_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2409);

PHP_METHOD(Stub_Issue2409, setEmpty);
PHP_METHOD(Stub_Issue2409, setNonEmpty);
PHP_METHOD(Stub_Issue2409, setNull);
PHP_METHOD(Stub_Issue2409, setInt);
PHP_METHOD(Stub_Issue2409, setNegativeInt);
PHP_METHOD(Stub_Issue2409, setDouble);
PHP_METHOD(Stub_Issue2409, setBoolTrue);
PHP_METHOD(Stub_Issue2409, setBoolFalse);
PHP_METHOD(Stub_Issue2409, setChar);
PHP_METHOD(Stub_Issue2409, setEmptyArray);
PHP_METHOD(Stub_Issue2409, setArray);
PHP_METHOD(Stub_Issue2409, setFromStringVariable);
PHP_METHOD(Stub_Issue2409, setFromIntVariable);
PHP_METHOD(Stub_Issue2409, setFromDoubleVariable);
PHP_METHOD(Stub_Issue2409, setFromBoolVariable);
PHP_METHOD(Stub_Issue2409, getVar);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2409_setempty, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2409_setnonempty, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2409_setnull, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2409_setint, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2409_setnegativeint, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2409_setdouble, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2409_setbooltrue, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2409_setboolfalse, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2409_setchar, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2409_setemptyarray, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2409_setarray, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2409_setfromstringvariable, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2409_setfromintvariable, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2409_setfromdoublevariable, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2409_setfromboolvariable, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2409_getvar, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2409_method_entry) {
	PHP_ME(Stub_Issue2409, setEmpty, arginfo_stub_issue2409_setempty, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2409, setNonEmpty, arginfo_stub_issue2409_setnonempty, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2409, setNull, arginfo_stub_issue2409_setnull, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2409, setInt, arginfo_stub_issue2409_setint, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2409, setNegativeInt, arginfo_stub_issue2409_setnegativeint, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2409, setDouble, arginfo_stub_issue2409_setdouble, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2409, setBoolTrue, arginfo_stub_issue2409_setbooltrue, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2409, setBoolFalse, arginfo_stub_issue2409_setboolfalse, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2409, setChar, arginfo_stub_issue2409_setchar, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2409, setEmptyArray, arginfo_stub_issue2409_setemptyarray, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2409, setArray, arginfo_stub_issue2409_setarray, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2409, setFromStringVariable, arginfo_stub_issue2409_setfromstringvariable, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2409, setFromIntVariable, arginfo_stub_issue2409_setfromintvariable, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2409, setFromDoubleVariable, arginfo_stub_issue2409_setfromdoublevariable, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2409, setFromBoolVariable, arginfo_stub_issue2409_setfromboolvariable, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
PHP_ME(Stub_Issue2409, getVar, arginfo_stub_issue2409_getvar, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
