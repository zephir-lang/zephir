
extern zend_class_entry *stub_attributes_marker_ce;

ZEPHIR_INIT_CLASS(Stub_Attributes_Marker);

PHP_METHOD(Stub_Attributes_Marker, __construct);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_attributes_marker___construct, 0, 0, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, text, IS_STRING, 0, "''")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, flag, _IS_BOOL, 0, "false")
ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, extra, IS_ARRAY, 0, "[]")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, number, IS_LONG, 0, "0")
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_attributes_marker_method_entry) {
	PHP_ME(Stub_Attributes_Marker, __construct, arginfo_stub_attributes_marker___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_FE_END
};
