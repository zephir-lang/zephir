
extern zend_class_entry *stub_fcallsafetybase_ce;

ZEPHIR_INIT_CLASS(Stub_FcallSafetyBase);

PHP_METHOD(Stub_FcallSafetyBase, poly);
PHP_METHOD(Stub_FcallSafetyBase, fin);
PHP_METHOD(Stub_FcallSafetyBase, priv);
PHP_METHOD(Stub_FcallSafetyBase, callPriv);
PHP_METHOD(Stub_FcallSafetyBase, stat);
PHP_METHOD(Stub_FcallSafetyBase, drive);
PHP_METHOD(Stub_FcallSafetyBase, fib);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_fcallsafetybase_poly, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_fcallsafetybase_fin, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_fcallsafetybase_priv, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_fcallsafetybase_callpriv, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_fcallsafetybase_stat, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_fcallsafetybase_drive, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_fcallsafetybase_fib, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_fcallsafetybase_method_entry) {
	PHP_ME(Stub_FcallSafetyBase, poly, arginfo_stub_fcallsafetybase_poly, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_FcallSafetyBase, fin, arginfo_stub_fcallsafetybase_fin, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Stub_FcallSafetyBase, priv, arginfo_stub_fcallsafetybase_priv, ZEND_ACC_PRIVATE)
	PHP_ME(Stub_FcallSafetyBase, callPriv, arginfo_stub_fcallsafetybase_callpriv, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_FcallSafetyBase, stat, arginfo_stub_fcallsafetybase_stat, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_FcallSafetyBase, drive, arginfo_stub_fcallsafetybase_drive, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_FcallSafetyBase, fib, arginfo_stub_fcallsafetybase_fib, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
