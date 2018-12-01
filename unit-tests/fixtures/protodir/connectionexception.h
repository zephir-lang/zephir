
extern zend_class_entry *protodir_connectionexception_ce;

ZEPHIR_INIT_CLASS(ProtoDir_ConnectionException);

PHP_METHOD(ProtoDir_ConnectionException, getRequest);

ZEPHIR_INIT_FUNCS(protodir_connectionexception_method_entry) {
	PHP_ME(ProtoDir_ConnectionException, getRequest, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
