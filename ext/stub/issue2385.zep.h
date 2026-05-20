
extern zend_class_entry *stub_issue2385_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2385);

PHP_METHOD(Stub_Issue2385, __construct);
PHP_METHOD(Stub_Issue2385, checkDeclaredOnly);
PHP_METHOD(Stub_Issue2385, checkAssignedNull);
PHP_METHOD(Stub_Issue2385, checkAssignedValue);
PHP_METHOD(Stub_Issue2385, checkDynamicKeyNull);
PHP_METHOD(Stub_Issue2385, checkArrayKeyNull);
PHP_METHOD(Stub_Issue2385, checkArrayKeyValue);
PHP_METHOD(Stub_Issue2385, checkArrayKeyMissing);
PHP_METHOD(Stub_Issue2385, checkArrayLongKeyNull);
PHP_METHOD(Stub_Issue2385, checkArrayLongKeyValue);
PHP_METHOD(Stub_Issue2385, checkArrayVarKeyNull);
PHP_METHOD(Stub_Issue2385, checkArrayVarKeyValue);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2385___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2385_checkdeclaredonly, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2385_checkassignednull, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2385_checkassignedvalue, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2385_checkdynamickeynull, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2385_checkarraykeynull, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2385_checkarraykeyvalue, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2385_checkarraykeymissing, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2385_checkarraylongkeynull, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2385_checkarraylongkeyvalue, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2385_checkarrayvarkeynull, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2385_checkarrayvarkeyvalue, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2385_method_entry) {
PHP_ME(Stub_Issue2385, __construct, arginfo_stub_issue2385___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Stub_Issue2385, checkDeclaredOnly, arginfo_stub_issue2385_checkdeclaredonly, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2385, checkAssignedNull, arginfo_stub_issue2385_checkassignednull, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2385, checkAssignedValue, arginfo_stub_issue2385_checkassignedvalue, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2385, checkDynamicKeyNull, arginfo_stub_issue2385_checkdynamickeynull, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2385, checkArrayKeyNull, arginfo_stub_issue2385_checkarraykeynull, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2385, checkArrayKeyValue, arginfo_stub_issue2385_checkarraykeyvalue, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2385, checkArrayKeyMissing, arginfo_stub_issue2385_checkarraykeymissing, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2385, checkArrayLongKeyNull, arginfo_stub_issue2385_checkarraylongkeynull, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2385, checkArrayLongKeyValue, arginfo_stub_issue2385_checkarraylongkeyvalue, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2385, checkArrayVarKeyNull, arginfo_stub_issue2385_checkarrayvarkeynull, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2385, checkArrayVarKeyValue, arginfo_stub_issue2385_checkarrayvarkeyvalue, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
