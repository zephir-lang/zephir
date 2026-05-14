
extern zend_class_entry *stub_properties_propertyupdate_ce;

ZEPHIR_INIT_CLASS(Stub_Properties_PropertyUpdate);

PHP_METHOD(Stub_Properties_PropertyUpdate, update1);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_properties_propertyupdate_update1, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_properties_propertyupdate_method_entry) {
PHP_ME(Stub_Properties_PropertyUpdate, update1, arginfo_stub_properties_propertyupdate_update1, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
