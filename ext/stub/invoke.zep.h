
extern zend_class_entry *stub_invoke_ce;

ZEPHIR_INIT_CLASS(Stub_Invoke);

PHP_METHOD(Stub_Invoke, __construct);
PHP_METHOD(Stub_Invoke, __invoke);
PHP_METHOD(Stub_Invoke, test);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_invoke___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_invoke___invoke, 0, 0, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_invoke___invoke, 0, 0, IS_STRING, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_invoke_test, 0, 0, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_invoke_test, 0, 0, IS_STRING, NULL, 0)
#endif
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_invoke_method_entry) {
	PHP_ME(Stub_Invoke, __construct, arginfo_stub_invoke___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Stub_Invoke, __invoke, arginfo_stub_invoke___invoke, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Invoke, test, arginfo_stub_invoke_test, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
