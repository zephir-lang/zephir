
extern zend_class_entry *stub_properties_getobjectvars_ce;

ZEPHIR_INIT_CLASS(Stub_Properties_GetObjectVars);

PHP_METHOD(Stub_Properties_GetObjectVars, issue1245);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_properties_getobjectvars_issue1245, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_properties_getobjectvars_method_entry) {
	PHP_ME(Stub_Properties_GetObjectVars, issue1245, arginfo_stub_properties_getobjectvars_issue1245, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
