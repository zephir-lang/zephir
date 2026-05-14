
extern zend_class_entry *stub_issue1134_ce;

ZEPHIR_INIT_CLASS(Stub_Issue1134);

PHP_METHOD(Stub_Issue1134, __construct);
PHP_METHOD(Stub_Issue1134, arrayWithFilledDefaultValue);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue1134___construct, 0, 0, 0)
ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, config, IS_ARRAY, 0, "[]")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue1134_arraywithfilleddefaultvalue, 0, 0, 0)
ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, someDefaultData, IS_ARRAY, 0, "[]")
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue1134_method_entry) {
	PHP_ME(Stub_Issue1134, __construct, arginfo_stub_issue1134___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Stub_Issue1134, arrayWithFilledDefaultValue, arginfo_stub_issue1134_arraywithfilleddefaultvalue, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
