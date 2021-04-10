
extern zend_class_entry *stub_issue663_ce;

ZEPHIR_INIT_CLASS(Stub_Issue663);

PHP_METHOD(Stub_Issue663, is_array_assoc);
PHP_METHOD(Stub_Issue663, is_array_assoc_internal);
PHP_METHOD(Stub_Issue663, is_array_indexed);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue663_is_array_assoc, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, arr)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue663_is_array_assoc_internal, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_ARRAY_INFO(0, arr, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue663_is_array_indexed, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, arr)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue663_method_entry) {
	PHP_ME(Stub_Issue663, is_array_assoc, arginfo_stub_issue663_is_array_assoc, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue663, is_array_assoc_internal, arginfo_stub_issue663_is_array_assoc_internal, ZEND_ACC_PRIVATE|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue663, is_array_indexed, arginfo_stub_issue663_is_array_indexed, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
