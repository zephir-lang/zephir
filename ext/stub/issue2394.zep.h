
extern zend_class_entry *stub_issue2394_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2394);

PHP_METHOD(Stub_Issue2394, helper);
PHP_METHOD(Stub_Issue2394, addition);
PHP_METHOD(Stub_Issue2394, subtraction);
PHP_METHOD(Stub_Issue2394, multiplication);
PHP_METHOD(Stub_Issue2394, division);
PHP_METHOD(Stub_Issue2394, modulo);
PHP_METHOD(Stub_Issue2394, concatenation);
PHP_METHOD(Stub_Issue2394, ternary);
PHP_METHOD(Stub_Issue2394, comparison);
PHP_METHOD(Stub_Issue2394, methodCall);
PHP_METHOD(Stub_Issue2394, backslashLoop);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2394_helper, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2394_addition, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2394_subtraction, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2394_multiplication, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2394_division, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2394_modulo, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2394_concatenation, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, a, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, b, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2394_ternary, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2394_comparison, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2394_methodcall, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2394_backslashloop, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, format, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2394_method_entry) {
PHP_ME(Stub_Issue2394, helper, arginfo_stub_issue2394_helper, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2394, addition, arginfo_stub_issue2394_addition, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2394, subtraction, arginfo_stub_issue2394_subtraction, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2394, multiplication, arginfo_stub_issue2394_multiplication, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2394, division, arginfo_stub_issue2394_division, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2394, modulo, arginfo_stub_issue2394_modulo, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2394, concatenation, arginfo_stub_issue2394_concatenation, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2394, ternary, arginfo_stub_issue2394_ternary, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2394, comparison, arginfo_stub_issue2394_comparison, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Issue2394, methodCall, arginfo_stub_issue2394_methodcall, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2394, backslashLoop, arginfo_stub_issue2394_backslashloop, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
