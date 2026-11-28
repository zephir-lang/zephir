
extern zend_class_entry *stub_issue2869directory_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2869Directory);

PHP_METHOD(Stub_Issue2869Directory, symlinks);
PHP_METHOD(Stub_Issue2869Directory, mask);
PHP_METHOD(Stub_Issue2869Directory, skipDots);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2869directory_symlinks, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2869directory_mask, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2869directory_skipdots, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2869directory_method_entry) {
	PHP_ME(Stub_Issue2869Directory, symlinks, arginfo_stub_issue2869directory_symlinks, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2869Directory, mask, arginfo_stub_issue2869directory_mask, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2869Directory, skipDots, arginfo_stub_issue2869directory_skipdots, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
