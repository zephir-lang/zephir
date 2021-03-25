
extern zend_class_entry *stub_invokes_abstractprotected_ce;

ZEPHIR_INIT_CLASS(Stub_Invokes_AbstractProtected);

PHP_METHOD(Stub_Invokes_AbstractProtected, protectedMethod1);
PHP_METHOD(Stub_Invokes_AbstractProtected, protectedMethod2);
PHP_METHOD(Stub_Invokes_AbstractProtected, renderArrayElements);
PHP_METHOD(Stub_Invokes_AbstractProtected, renderArrayElementsComplex);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_invokes_abstractprotected_protectedmethod1, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, text, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_invokes_abstractprotected_protectedmethod2, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, text, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_invokes_abstractprotected_renderarrayelements, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, text, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_invokes_abstractprotected_renderarrayelementscomplex, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, text, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_invokes_abstractprotected_method_entry) {
	PHP_ME(Stub_Invokes_AbstractProtected, protectedMethod1, arginfo_stub_invokes_abstractprotected_protectedmethod1, ZEND_ACC_PROTECTED)
	PHP_ME(Stub_Invokes_AbstractProtected, protectedMethod2, arginfo_stub_invokes_abstractprotected_protectedmethod2, ZEND_ACC_PROTECTED)
	PHP_ME(Stub_Invokes_AbstractProtected, renderArrayElements, arginfo_stub_invokes_abstractprotected_renderarrayelements, ZEND_ACC_PROTECTED)
	PHP_ME(Stub_Invokes_AbstractProtected, renderArrayElementsComplex, arginfo_stub_invokes_abstractprotected_renderarrayelementscomplex, ZEND_ACC_PROTECTED)
	PHP_FE_END
};
