
extern zend_class_entry *stub_issue2869connection_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2869Connection);

PHP_METHOD(Stub_Issue2869Connection, unique);
PHP_METHOD(Stub_Issue2869Connection, group);
PHP_METHOD(Stub_Issue2869Connection, assoc);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2869connection_unique, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2869connection_group, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2869connection_assoc, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2869connection_method_entry) {
	PHP_ME(Stub_Issue2869Connection, unique, arginfo_stub_issue2869connection_unique, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2869Connection, group, arginfo_stub_issue2869connection_group, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2869Connection, assoc, arginfo_stub_issue2869connection_assoc, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
