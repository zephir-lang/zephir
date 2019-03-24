
extern zend_class_entry *test_arrayiterator_ce;

ZEPHIR_INIT_CLASS(Test_ArrayIterator);

PHP_METHOD(Test_ArrayIterator, __construct);
PHP_METHOD(Test_ArrayIterator, rewind);
PHP_METHOD(Test_ArrayIterator, current);
PHP_METHOD(Test_ArrayIterator, key);
PHP_METHOD(Test_ArrayIterator, next);
PHP_METHOD(Test_ArrayIterator, valid);

ZEPHIR_INIT_FUNCS(test_arrayiterator_method_entry) {
	PHP_ME(Test_ArrayIterator, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Test_ArrayIterator, rewind, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_ArrayIterator, current, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_ArrayIterator, key, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_ArrayIterator, next, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_ArrayIterator, valid, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
