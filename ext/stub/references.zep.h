
extern zend_class_entry *stub_references_ce;

ZEPHIR_INIT_CLASS(Stub_References);

PHP_METHOD(Stub_References, assignByRef);

ZEPHIR_INIT_FUNCS(stub_references_method_entry) {
	PHP_ME(Stub_References, assignByRef, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
