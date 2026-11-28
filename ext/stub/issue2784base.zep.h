
extern zend_class_entry *stub_issue2784base_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2784Base);

PHP_METHOD(Stub_Issue2784Base, unsetOwnPrivate);
PHP_METHOD(Stub_Issue2784Base, closureReadingPrivate);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2784base_unsetownprivate, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2784base_closurereadingprivate, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2784base_method_entry) {
	PHP_ME(Stub_Issue2784Base, unsetOwnPrivate, arginfo_stub_issue2784base_unsetownprivate, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Issue2784Base, closureReadingPrivate, arginfo_stub_issue2784base_closurereadingprivate, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
