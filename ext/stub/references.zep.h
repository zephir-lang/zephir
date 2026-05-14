
extern zend_class_entry *stub_references_ce;

ZEPHIR_INIT_CLASS(Stub_References);

PHP_METHOD(Stub_References, assignByRef);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_references_assignbyref, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_references_method_entry) {
PHP_ME(Stub_References, assignByRef, arginfo_stub_references_assignbyref, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
