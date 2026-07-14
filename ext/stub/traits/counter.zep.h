
extern zend_class_entry *stub_traits_counter_ce;

ZEPHIR_INIT_CLASS(Stub_Traits_Counter);

PHP_METHOD(Stub_Traits_Counter, increment);
PHP_METHOD(Stub_Traits_Counter, describe);
PHP_METHOD(Stub_Traits_Counter, label);
PHP_METHOD(Stub_Traits_Counter, tagged);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_traits_counter_increment, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_traits_counter_describe, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_traits_counter_label, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_traits_counter_tagged, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_traits_counter_method_entry) {
	PHP_ME(Stub_Traits_Counter, increment, arginfo_stub_traits_counter_increment, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Traits_Counter, describe, arginfo_stub_traits_counter_describe, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Traits_Counter, label, arginfo_stub_traits_counter_label, ZEND_ACC_ABSTRACT|ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Traits_Counter, tagged, arginfo_stub_traits_counter_tagged, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
