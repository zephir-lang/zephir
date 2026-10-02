
extern zend_class_entry *stub_arrayread_ce;

ZEPHIR_INIT_CLASS(Stub_ArrayRead);

PHP_METHOD(Stub_ArrayRead, read);
PHP_METHOD(Stub_ArrayRead, readFromArray);
PHP_METHOD(Stub_ArrayRead, readStringLiteral);
PHP_METHOD(Stub_ArrayRead, readIntLiteral);
PHP_METHOD(Stub_ArrayRead, readLong);
PHP_METHOD(Stub_ArrayRead, readString);
PHP_METHOD(Stub_ArrayRead, readNested);
PHP_METHOD(Stub_ArrayRead, readNestedLiteral);
PHP_METHOD(Stub_ArrayRead, readIntoLocal);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_arrayread_read, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, k)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_arrayread_readfromarray, 0, 0, 2)
	ZEND_ARG_ARRAY_INFO(0, a, 0)
	ZEND_ARG_INFO(0, k)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_arrayread_readstringliteral, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_arrayread_readintliteral, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_arrayread_readlong, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_TYPE_INFO(0, k, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_arrayread_readstring, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_TYPE_INFO(0, k, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_arrayread_readnested, 0, 0, 3)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, i)
	ZEND_ARG_INFO(0, j)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_arrayread_readnestedliteral, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_arrayread_readintolocal, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, k)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_arrayread_method_entry) {
	PHP_ME(Stub_ArrayRead, read, arginfo_stub_arrayread_read, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayRead, readFromArray, arginfo_stub_arrayread_readfromarray, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayRead, readStringLiteral, arginfo_stub_arrayread_readstringliteral, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayRead, readIntLiteral, arginfo_stub_arrayread_readintliteral, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayRead, readLong, arginfo_stub_arrayread_readlong, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayRead, readString, arginfo_stub_arrayread_readstring, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayRead, readNested, arginfo_stub_arrayread_readnested, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayRead, readNestedLiteral, arginfo_stub_arrayread_readnestedliteral, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayRead, readIntoLocal, arginfo_stub_arrayread_readintolocal, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
