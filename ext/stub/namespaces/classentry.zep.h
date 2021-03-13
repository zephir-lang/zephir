
extern zend_class_entry *stub_namespaces_classentry_ce;

ZEPHIR_INIT_CLASS(Stub_Namespaces_ClassEntry);

PHP_METHOD(Stub_Namespaces_ClassEntry, setParam);
PHP_METHOD(Stub_Namespaces_ClassEntry, setParamImported);

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_namespaces_classentry_setparam, 0, 1, Stub\\Namespaces\\A\\B\\Sub, 0)
	ZEND_ARG_OBJ_INFO(0, param, Stub\\Namespaces\\A\\B\\Sub, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_namespaces_classentry_setparamimported, 0, 1, Stub\\Namespaces\\A\\B\\Sub, 0)
	ZEND_ARG_OBJ_INFO(0, param, Stub\\Namespaces\\A\\B\\Sub, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_namespaces_classentry_method_entry) {
	PHP_ME(Stub_Namespaces_ClassEntry, setParam, arginfo_stub_namespaces_classentry_setparam, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Namespaces_ClassEntry, setParamImported, arginfo_stub_namespaces_classentry_setparamimported, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
