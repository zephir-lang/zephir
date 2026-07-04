
extern zend_class_entry *stub_traits_counteruser_ce;

ZEPHIR_INIT_CLASS(Stub_Traits_CounterUser);

PHP_METHOD(Stub_Traits_CounterUser, label);
PHP_METHOD(Stub_Traits_CounterUser, increment);
PHP_METHOD(Stub_Traits_CounterUser, describe);
PHP_METHOD(Stub_Traits_CounterUser, tagged);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_traits_counteruser_label, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_traits_counteruser_increment, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_traits_counteruser_describe, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_traits_counteruser_tagged, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_traits_counteruser_method_entry) {
	PHP_ME(Stub_Traits_CounterUser, label, arginfo_stub_traits_counteruser_label, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Traits_CounterUser, increment, arginfo_stub_traits_counteruser_increment, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Traits_CounterUser, describe, arginfo_stub_traits_counteruser_describe, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Traits_CounterUser, tagged, arginfo_stub_traits_counteruser_tagged, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
