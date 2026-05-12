
extern zend_class_entry *stub_issue2469_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2469);

PHP_METHOD(Stub_Issue2469, peek);
PHP_METHOD(Stub_Issue2469, callWithLiteral);
PHP_METHOD(Stub_Issue2469, callWithVariable);
PHP_METHOD(Stub_Issue2469, callWithLiteralUchar);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2469_peek, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, ch, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2469_callwithliteral, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2469_callwithvariable, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2469_callwithliteraluchar, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2469_method_entry) {
	PHP_ME(Stub_Issue2469, peek, arginfo_stub_issue2469_peek, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2469, callWithLiteral, arginfo_stub_issue2469_callwithliteral, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2469, callWithVariable, arginfo_stub_issue2469_callwithvariable, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2469, callWithLiteralUchar, arginfo_stub_issue2469_callwithliteraluchar, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
