
extern zend_class_entry *stub_functions_ce;

ZEPHIR_INIT_CLASS(Stub_Functions);

PHP_METHOD(Stub_Functions, filterVar1);
PHP_METHOD(Stub_Functions, filterVar2);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_functions_filtervar1, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_functions_filtervar2, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_functions_method_entry) {
	PHP_ME(Stub_Functions, filterVar1, arginfo_stub_functions_filtervar1, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Functions, filterVar2, arginfo_stub_functions_filtervar2, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
