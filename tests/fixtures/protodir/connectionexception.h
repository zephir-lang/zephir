
extern zend_class_entry *protodir_connectionexception_ce;

ZEPHIR_INIT_CLASS(ProtoDir_ConnectionException);

PHP_METHOD(ProtoDir_ConnectionException, getRequest);

ZEND_BEGIN_ARG_INFO_EX(arginfo_protodir_connectionexception_getrequest, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(protodir_connectionexception_method_entry) {
#if PHP_VERSION_ID >= 80000
	PHP_ME(ProtoDir_ConnectionException, getRequest, arginfo_protodir_connectionexception_getrequest, ZEND_ACC_PUBLIC)
#else
	PHP_ME(ProtoDir_ConnectionException, getRequest, NULL, ZEND_ACC_PUBLIC)
#endif
	PHP_FE_END
};
