
extern zend_class_entry *stub_issue2537child_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2537Child);

PHP_METHOD(Stub_Issue2537Child, make);
PHP_METHOD(Stub_Issue2537Child, makeNullable);
PHP_METHOD(Stub_Issue2537Child, makeViaParent);
PHP_METHOD(Stub_Issue2537Child, makeViaSelf);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_stub_issue2537child_make, 0, 0, MAY_BE_STATIC)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_stub_issue2537child_makenullable, 0, 0, MAY_BE_NULL|MAY_BE_STATIC)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_issue2537child_makeviaparent, 0, 0, parent, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_issue2537child_makeviaself, 0, 0, self, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2537child_method_entry) {
	PHP_ME(Stub_Issue2537Child, make, arginfo_stub_issue2537child_make, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2537Child, makeNullable, arginfo_stub_issue2537child_makenullable, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2537Child, makeViaParent, arginfo_stub_issue2537child_makeviaparent, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2537Child, makeViaSelf, arginfo_stub_issue2537child_makeviaself, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
