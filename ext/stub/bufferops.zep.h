
extern zend_class_entry *stub_bufferops_ce;

ZEPHIR_INIT_CLASS(Stub_BufferOps);

PHP_METHOD(Stub_BufferOps, readAt);
PHP_METHOD(Stub_BufferOps, writeAt);
PHP_METHOD(Stub_BufferOps, has);
PHP_METHOD(Stub_BufferOps, size);
PHP_METHOD(Stub_BufferOps, sum);
PHP_METHOD(Stub_BufferOps, scale);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_bufferops_readat, 0, 0, 2)
	ZEND_ARG_INFO(0, buf)
	ZEND_ARG_TYPE_INFO(0, index, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_bufferops_writeat, 0, 3, IS_VOID, 0)

	ZEND_ARG_INFO(0, buf)
	ZEND_ARG_TYPE_INFO(0, index, IS_LONG, 0)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_bufferops_has, 0, 2, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, buf)
	ZEND_ARG_TYPE_INFO(0, index, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_bufferops_size, 0, 1, IS_LONG, 0)
	ZEND_ARG_INFO(0, buf)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_bufferops_sum, 0, 1, IS_DOUBLE, 0)
	ZEND_ARG_INFO(0, buf)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_bufferops_scale, 0, 2, IS_VOID, 0)

	ZEND_ARG_INFO(0, buf)
	ZEND_ARG_TYPE_INFO(0, factor, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_bufferops_method_entry) {
	PHP_ME(Stub_BufferOps, readAt, arginfo_stub_bufferops_readat, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BufferOps, writeAt, arginfo_stub_bufferops_writeat, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BufferOps, has, arginfo_stub_bufferops_has, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BufferOps, size, arginfo_stub_bufferops_size, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BufferOps, sum, arginfo_stub_bufferops_sum, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_BufferOps, scale, arginfo_stub_bufferops_scale, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
