
extern zend_class_entry *stub_unreachableloop_ce;

ZEPHIR_INIT_CLASS(Stub_UnreachableLoop);

PHP_METHOD(Stub_UnreachableLoop, toPropertyString);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_unreachableloop_topropertystring, 0, 1, IS_STRING, 0)
	ZEND_ARG_ARRAY_INFO(0, properties, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_unreachableloop_method_entry) {
	PHP_ME(Stub_UnreachableLoop, toPropertyString, arginfo_stub_unreachableloop_topropertystring, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
