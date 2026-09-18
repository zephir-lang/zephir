
extern zend_class_entry *stub_attributes_counter_ce;

ZEPHIR_INIT_CLASS(Stub_Attributes_Counter);

PHP_METHOD(Stub_Attributes_Counter, bump);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_attributes_counter_bump, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_attributes_counter_method_entry) {
	PHP_ME(Stub_Attributes_Counter, bump, arginfo_stub_attributes_counter_bump, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
