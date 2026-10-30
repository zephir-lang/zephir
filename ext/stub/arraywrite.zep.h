
extern zend_class_entry *stub_arraywrite_ce;

ZEPHIR_INIT_CLASS(Stub_ArrayWrite);

PHP_METHOD(Stub_ArrayWrite, write);
PHP_METHOD(Stub_ArrayWrite, writeStringLiteral);
PHP_METHOD(Stub_ArrayWrite, writeIntLiteral);
PHP_METHOD(Stub_ArrayWrite, writeLong);
PHP_METHOD(Stub_ArrayWrite, writeNative);
PHP_METHOD(Stub_ArrayWrite, writeNested);
PHP_METHOD(Stub_ArrayWrite, append);
PHP_METHOD(Stub_ArrayWrite, appendNative);
PHP_METHOD(Stub_ArrayWrite, appendNested);
PHP_METHOD(Stub_ArrayWrite, writeThis);
PHP_METHOD(Stub_ArrayWrite, writeThisNested);
PHP_METHOD(Stub_ArrayWrite, appendThis);
PHP_METHOD(Stub_ArrayWrite, writeStatic);
PHP_METHOD(Stub_ArrayWrite, appendStatic);
PHP_METHOD(Stub_ArrayWrite, concatDeep);
PHP_METHOD(Stub_ArrayWrite, writeObject);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_arraywrite_write, 0, 0, 3)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, k)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_arraywrite_writestringliteral, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_arraywrite_writeintliteral, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_arraywrite_writelong, 0, 0, 3)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_TYPE_INFO(0, k, IS_LONG, 0)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_arraywrite_writenative, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, k)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_arraywrite_writenested, 0, 0, 4)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, i)
	ZEND_ARG_INFO(0, j)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_arraywrite_append, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_arraywrite_appendnative, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_arraywrite_appendnested, 0, 0, 3)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, k)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_arraywrite_writethis, 0, 0, 3)
	ZEND_ARG_INFO(0, p)
	ZEND_ARG_INFO(0, k)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_arraywrite_writethisnested, 0, 0, 4)
	ZEND_ARG_INFO(0, p)
	ZEND_ARG_INFO(0, i)
	ZEND_ARG_INFO(0, j)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_arraywrite_appendthis, 0, 0, 2)
	ZEND_ARG_INFO(0, p)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_arraywrite_writestatic, 0, 0, 3)
	ZEND_ARG_INFO(0, p)
	ZEND_ARG_INFO(0, k)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_arraywrite_appendstatic, 0, 0, 2)
	ZEND_ARG_INFO(0, p)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_arraywrite_concatdeep, 0, 0, 5)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, i)
	ZEND_ARG_INFO(0, j)
	ZEND_ARG_INFO(0, k)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_arraywrite_writeobject, 0, 0, 3)
	ZEND_ARG_INFO(0, o)
	ZEND_ARG_INFO(0, k)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_arraywrite_method_entry) {
	PHP_ME(Stub_ArrayWrite, write, arginfo_stub_arraywrite_write, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayWrite, writeStringLiteral, arginfo_stub_arraywrite_writestringliteral, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayWrite, writeIntLiteral, arginfo_stub_arraywrite_writeintliteral, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayWrite, writeLong, arginfo_stub_arraywrite_writelong, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayWrite, writeNative, arginfo_stub_arraywrite_writenative, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayWrite, writeNested, arginfo_stub_arraywrite_writenested, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayWrite, append, arginfo_stub_arraywrite_append, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayWrite, appendNative, arginfo_stub_arraywrite_appendnative, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayWrite, appendNested, arginfo_stub_arraywrite_appendnested, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayWrite, writeThis, arginfo_stub_arraywrite_writethis, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayWrite, writeThisNested, arginfo_stub_arraywrite_writethisnested, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayWrite, appendThis, arginfo_stub_arraywrite_appendthis, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayWrite, writeStatic, arginfo_stub_arraywrite_writestatic, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayWrite, appendStatic, arginfo_stub_arraywrite_appendstatic, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayWrite, concatDeep, arginfo_stub_arraywrite_concatdeep, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayWrite, writeObject, arginfo_stub_arraywrite_writeobject, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
