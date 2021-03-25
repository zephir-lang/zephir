
extern zend_class_entry *stub_evaltest_ce;

ZEPHIR_INIT_CLASS(Stub_EvalTest);

PHP_METHOD(Stub_EvalTest, evalCode);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_evaltest_evalcode, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, code, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_evaltest_method_entry) {
	PHP_ME(Stub_EvalTest, evalCode, arginfo_stub_evaltest_evalcode, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
