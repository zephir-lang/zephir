
extern zend_class_entry *stub_issue2537_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2537);

PHP_METHOD(Stub_Issue2537, make);
PHP_METHOD(Stub_Issue2537, makeNullable);
PHP_METHOD(Stub_Issue2537, getTag);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_stub_issue2537_make, 0, 0, MAY_BE_STATIC)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_stub_issue2537_makenullable, 0, 0, MAY_BE_NULL|MAY_BE_STATIC)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2537_gettag, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2537_method_entry) {
	PHP_ME(Stub_Issue2537, make, arginfo_stub_issue2537_make, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2537, makeNullable, arginfo_stub_issue2537_makenullable, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2537, getTag, arginfo_stub_issue2537_gettag, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
