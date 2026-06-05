
extern zend_class_entry *stub_fcallsafetychild_ce;

ZEPHIR_INIT_CLASS(Stub_FcallSafetyChild);

PHP_METHOD(Stub_FcallSafetyChild, poly);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_fcallsafetychild_poly, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_fcallsafetychild_method_entry) {
	PHP_ME(Stub_FcallSafetyChild, poly, arginfo_stub_fcallsafetychild_poly, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
