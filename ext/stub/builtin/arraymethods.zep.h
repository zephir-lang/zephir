
extern zend_class_entry *stub_builtin_arraymethods_ce;

ZEPHIR_INIT_CLASS(Stub_BuiltIn_ArrayMethods);

PHP_METHOD(Stub_BuiltIn_ArrayMethods, getJoin1);
PHP_METHOD(Stub_BuiltIn_ArrayMethods, getReversed1);
PHP_METHOD(Stub_BuiltIn_ArrayMethods, getMap1);
PHP_METHOD(Stub_BuiltIn_ArrayMethods, issue733BuiltInJoinSpecialChars);
PHP_METHOD(Stub_BuiltIn_ArrayMethods, issue733JoinOnVar);
PHP_METHOD(Stub_BuiltIn_ArrayMethods, issue733ReversedOnVar);
PHP_METHOD(Stub_BuiltIn_ArrayMethods, issue733JoinOnLiteralArray);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_arraymethods_getjoin1, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_arraymethods_getreversed1, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_builtin_arraymethods_getmap1, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_builtin_arraymethods_issue733builtinjoinspecialchars, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, a, IS_STRING, 0)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_builtin_arraymethods_issue733joinonvar, 0, 1, IS_STRING, 0)
	ZEND_ARG_INFO(0, input)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_builtin_arraymethods_issue733reversedonvar, 0, 1, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, input)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_builtin_arraymethods_issue733joinonliteralarray, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_builtin_arraymethods_method_entry) {
PHP_ME(Stub_BuiltIn_ArrayMethods, getJoin1, arginfo_stub_builtin_arraymethods_getjoin1, ZEND_ACC_PUBLIC)
PHP_ME(Stub_BuiltIn_ArrayMethods, getReversed1, arginfo_stub_builtin_arraymethods_getreversed1, ZEND_ACC_PUBLIC)
PHP_ME(Stub_BuiltIn_ArrayMethods, getMap1, arginfo_stub_builtin_arraymethods_getmap1, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BuiltIn_ArrayMethods, issue733BuiltInJoinSpecialChars, arginfo_stub_builtin_arraymethods_issue733builtinjoinspecialchars, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BuiltIn_ArrayMethods, issue733JoinOnVar, arginfo_stub_builtin_arraymethods_issue733joinonvar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BuiltIn_ArrayMethods, issue733ReversedOnVar, arginfo_stub_builtin_arraymethods_issue733reversedonvar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BuiltIn_ArrayMethods, issue733JoinOnLiteralArray, arginfo_stub_builtin_arraymethods_issue733joinonliteralarray, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
