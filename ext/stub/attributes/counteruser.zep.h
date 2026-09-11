
extern zend_class_entry *stub_attributes_counteruser_ce;

ZEPHIR_INIT_CLASS(Stub_Attributes_CounterUser);

PHP_METHOD(Stub_Attributes_CounterUser, bump);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_attributes_counteruser_bump, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_attributes_counteruser_method_entry) {
	PHP_ME(Stub_Attributes_CounterUser, bump, arginfo_stub_attributes_counteruser_bump, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
