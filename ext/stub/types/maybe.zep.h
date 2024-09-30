
extern zend_class_entry *stub_types_maybe_ce;

ZEPHIR_INIT_CLASS(Stub_Types_MayBe);

PHP_METHOD(Stub_Types_MayBe, gc);
PHP_METHOD(Stub_Types_MayBe, gcFalse);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_stub_types_maybe_gc, 0, 1, MAY_BE_LONG|MAY_BE_FALSE)
	ZEND_ARG_TYPE_INFO(0, maxlifetime, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_stub_types_maybe_gcfalse, 0, 0, MAY_BE_LONG|MAY_BE_FALSE)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_types_maybe_method_entry) {
	PHP_ME(Stub_Types_MayBe, gc, arginfo_stub_types_maybe_gc, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Types_MayBe, gcFalse, arginfo_stub_types_maybe_gcfalse, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
