
extern zend_class_entry *stub_17__closure_ce;

ZEPHIR_INIT_CLASS(stub_17__closure);

PHP_METHOD(stub_17__closure, __invoke);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_17__closure___invoke, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, value, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_17__closure_method_entry) {
	PHP_ME(stub_17__closure, __invoke, arginfo_stub_17__closure___invoke, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_FE_END
};
