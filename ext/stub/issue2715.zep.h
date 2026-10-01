
extern zend_class_entry *stub_issue2715_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2715);

PHP_METHOD(Stub_Issue2715, callMixedCase);
PHP_METHOD(Stub_Issue2715, callDynamicStringName);
PHP_METHOD(Stub_Issue2715, callDynamicVariableName);
PHP_METHOD(Stub_Issue2715, callStaticMixedCase);
PHP_METHOD(Stub_Issue2715, callStaticDynamicName);
PHP_METHOD(Stub_Issue2715, callServiceGetter);
PHP_METHOD(Stub_Issue2715, callDeclaredMethod);
PHP_METHOD(Stub_Issue2715, staticControl);
PHP_METHOD(Stub_Issue2715, callSelfMixedCase);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2715_callmixedcase, 0, 0, 1)
	ZEND_ARG_INFO(0, receiver)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2715_calldynamicstringname, 0, 0, 1)
	ZEND_ARG_INFO(0, receiver)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2715_calldynamicvariablename, 0, 0, 2)
	ZEND_ARG_INFO(0, receiver)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2715_callstaticmixedcase, 0, 0, 1)
	ZEND_ARG_INFO(0, className)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2715_callstaticdynamicname, 0, 0, 1)
	ZEND_ARG_INFO(0, className)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2715_callservicegetter, 0, 0, 1)
	ZEND_ARG_INFO(0, receiver)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2715_calldeclaredmethod, 0, 0, 1)
	ZEND_ARG_INFO(0, receiver)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2715_staticcontrol, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2715_callselfmixedcase, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2715_method_entry) {
	PHP_ME(Stub_Issue2715, callMixedCase, arginfo_stub_issue2715_callmixedcase, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2715, callDynamicStringName, arginfo_stub_issue2715_calldynamicstringname, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2715, callDynamicVariableName, arginfo_stub_issue2715_calldynamicvariablename, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2715, callStaticMixedCase, arginfo_stub_issue2715_callstaticmixedcase, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2715, callStaticDynamicName, arginfo_stub_issue2715_callstaticdynamicname, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2715, callServiceGetter, arginfo_stub_issue2715_callservicegetter, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2715, callDeclaredMethod, arginfo_stub_issue2715_calldeclaredmethod, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2715, staticControl, arginfo_stub_issue2715_staticcontrol, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2715, callSelfMixedCase, arginfo_stub_issue2715_callselfmixedcase, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
