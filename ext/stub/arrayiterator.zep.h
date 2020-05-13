
extern zend_class_entry *stub_arrayiterator_ce;

ZEPHIR_INIT_CLASS(Stub_ArrayIterator);

PHP_METHOD(Stub_ArrayIterator, __construct);
PHP_METHOD(Stub_ArrayIterator, rewind);
PHP_METHOD(Stub_ArrayIterator, current);
PHP_METHOD(Stub_ArrayIterator, key);
PHP_METHOD(Stub_ArrayIterator, next);
PHP_METHOD(Stub_ArrayIterator, valid);

ZEPHIR_INIT_FUNCS(stub_arrayiterator_method_entry) {
	PHP_ME(Stub_ArrayIterator, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Stub_ArrayIterator, rewind, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayIterator, current, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayIterator, key, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayIterator, next, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayIterator, valid, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
