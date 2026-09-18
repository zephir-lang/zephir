
extern zend_class_entry *stub_attributes_tools_ce;

ZEPHIR_INIT_CLASS(Stub_Attributes_Tools);

PHP_METHOD(Stub_Attributes_Tools, callTagged);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_attributes_tools_calltagged, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_attributes_tools_method_entry) {
	PHP_ME(Stub_Attributes_Tools, callTagged, arginfo_stub_attributes_tools_calltagged, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
