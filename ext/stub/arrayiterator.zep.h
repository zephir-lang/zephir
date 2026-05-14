
extern zend_class_entry *stub_arrayiterator_ce;

ZEPHIR_INIT_CLASS(Stub_ArrayIterator);

PHP_METHOD(Stub_ArrayIterator, __construct);
PHP_METHOD(Stub_ArrayIterator, rewind);
PHP_METHOD(Stub_ArrayIterator, current);
PHP_METHOD(Stub_ArrayIterator, key);
PHP_METHOD(Stub_ArrayIterator, next);
PHP_METHOD(Stub_ArrayIterator, valid);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_arrayiterator___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_arrayiterator_rewind, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_arrayiterator_current, 0, 0, IS_MIXED, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_arrayiterator_key, 0, 0, IS_MIXED, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_arrayiterator_next, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_arrayiterator_valid, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_arrayiterator_method_entry) {
PHP_ME(Stub_ArrayIterator, __construct, arginfo_stub_arrayiterator___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Stub_ArrayIterator, rewind, arginfo_stub_arrayiterator_rewind, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayIterator, current, arginfo_stub_arrayiterator_current, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayIterator, key, arginfo_stub_arrayiterator_key, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayIterator, next, arginfo_stub_arrayiterator_next, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayIterator, valid, arginfo_stub_arrayiterator_valid, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
