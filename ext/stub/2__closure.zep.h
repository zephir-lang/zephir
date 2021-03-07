
extern zend_class_entry *stub_2__closure_ce;

ZEPHIR_INIT_CLASS(stub_2__closure);

PHP_METHOD(stub_2__closure, __invoke);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_2__closure___invoke, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_2__closure_method_entry) {
#if PHP_VERSION_ID >= 80000
	PHP_ME(stub_2__closure, __invoke, arginfo_stub_2__closure___invoke, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#else
	PHP_ME(stub_2__closure, __invoke, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
#endif
	PHP_FE_END
};
