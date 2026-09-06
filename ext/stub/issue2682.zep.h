
extern zend_class_entry *stub_issue2682_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2682);

PHP_METHOD(Stub_Issue2682, fetchReleasedEarly);
PHP_METHOD(Stub_Issue2682, fetchOnly);
PHP_METHOD(Stub_Issue2682, readReleasedEarly);
PHP_METHOD(Stub_Issue2682, plainRead);
PHP_METHOD(Stub_Issue2682, leakProbe);
PHP_METHOD(Stub_Issue2682, leakProbeControl);
PHP_METHOD(Stub_Issue2682, nativeArrayStaysReadOnly);
zend_object *zephir_init_properties_Stub_Issue2682(zend_class_entry *class_type);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2682_fetchreleasedearly, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, container)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2682_fetchonly, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, container)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2682_readreleasedearly, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, container)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2682_plainread, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, container)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2682_leakprobe, 0, 2, IS_LONG, 0)
	ZEND_ARG_INFO(0, container)
	ZEND_ARG_TYPE_INFO(0, iterations, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2682_leakprobecontrol, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, iterations, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2682_nativearraystaysreadonly, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, mode, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2682_zephir_init_properties_stub_issue2682, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2682_method_entry) {
	PHP_ME(Stub_Issue2682, fetchReleasedEarly, arginfo_stub_issue2682_fetchreleasedearly, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2682, fetchOnly, arginfo_stub_issue2682_fetchonly, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2682, readReleasedEarly, arginfo_stub_issue2682_readreleasedearly, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2682, plainRead, arginfo_stub_issue2682_plainread, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2682, leakProbe, arginfo_stub_issue2682_leakprobe, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2682, leakProbeControl, arginfo_stub_issue2682_leakprobecontrol, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2682, nativeArrayStaysReadOnly, arginfo_stub_issue2682_nativearraystaysreadonly, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
