
extern zend_class_entry *stub_issue2784magic_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2784Magic);

PHP_METHOD(Stub_Issue2784Magic, __get);
PHP_METHOD(Stub_Issue2784Magic, readParentPrivate);
PHP_METHOD(Stub_Issue2784Magic, returnParentPrivate);
PHP_METHOD(Stub_Issue2784Magic, returnVirtual);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2784magic___get, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2784magic_readparentprivate, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2784magic_returnparentprivate, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2784magic_returnvirtual, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2784magic_method_entry) {
	PHP_ME(Stub_Issue2784Magic, __get, arginfo_stub_issue2784magic___get, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Issue2784Magic, readParentPrivate, arginfo_stub_issue2784magic_readparentprivate, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Issue2784Magic, returnParentPrivate, arginfo_stub_issue2784magic_returnparentprivate, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Issue2784Magic, returnVirtual, arginfo_stub_issue2784magic_returnvirtual, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
