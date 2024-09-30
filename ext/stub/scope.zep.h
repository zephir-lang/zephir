
extern zend_class_entry *stub_scope_ce;

ZEPHIR_INIT_CLASS(Stub_Scope);

PHP_METHOD(Stub_Scope, getStr);
PHP_METHOD(Stub_Scope, getDyStr);
PHP_METHOD(Stub_Scope, test1);
PHP_METHOD(Stub_Scope, test2);
PHP_METHOD(Stub_Scope, test3);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_scope_getstr, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_scope_getdystr, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, g, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_scope_test1, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_scope_test2, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_scope_test3, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_scope_method_entry) {
PHP_ME(Stub_Scope, getStr, arginfo_stub_scope_getstr, ZEND_ACC_PRIVATE|ZEND_ACC_STATIC)
	PHP_ME(Stub_Scope, getDyStr, arginfo_stub_scope_getdystr, ZEND_ACC_PRIVATE|ZEND_ACC_STATIC)
PHP_ME(Stub_Scope, test1, arginfo_stub_scope_test1, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
PHP_ME(Stub_Scope, test2, arginfo_stub_scope_test2, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
PHP_ME(Stub_Scope, test3, arginfo_stub_scope_test3, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
