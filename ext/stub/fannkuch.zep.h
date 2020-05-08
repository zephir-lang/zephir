
extern zend_class_entry *stub_fannkuch_ce;

ZEPHIR_INIT_CLASS(Stub_Fannkuch);

PHP_METHOD(Stub_Fannkuch, process);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_fannkuch_process, 0, 0, 1)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, n)
#endif
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_fannkuch_method_entry) {
	PHP_ME(Stub_Fannkuch, process, arginfo_stub_fannkuch_process, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
