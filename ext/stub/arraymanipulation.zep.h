
extern zend_class_entry *stub_arraymanipulation_ce;

ZEPHIR_INIT_CLASS(Stub_ArrayManipulation);

PHP_METHOD(Stub_ArrayManipulation, arrayFillMany);
PHP_METHOD(Stub_ArrayManipulation, issue774);
zend_object *zephir_init_properties_Stub_ArrayManipulation(zend_class_entry *class_type);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_arraymanipulation_arrayfillmany, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_arraymanipulation_issue774, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_arraymanipulation_zephir_init_properties_stub_arraymanipulation, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_arraymanipulation_method_entry) {
	PHP_ME(Stub_ArrayManipulation, arrayFillMany, arginfo_stub_arraymanipulation_arrayfillmany, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayManipulation, issue774, arginfo_stub_arraymanipulation_issue774, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
