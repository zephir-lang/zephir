
extern zend_class_entry *stub_attributes_demo_ce;

ZEPHIR_INIT_CLASS(Stub_Attributes_Demo);

PHP_METHOD(Stub_Attributes_Demo, getValue);
PHP_METHOD(Stub_Attributes_Demo, plain);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_attributes_demo_getvalue, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, token, IS_STRING, 0, "''")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_attributes_demo_plain, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_attributes_demo_method_entry) {
	PHP_ME(Stub_Attributes_Demo, getValue, arginfo_stub_attributes_demo_getvalue, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Attributes_Demo, plain, arginfo_stub_attributes_demo_plain, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
