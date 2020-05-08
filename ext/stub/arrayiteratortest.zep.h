
extern zend_class_entry *stub_arrayiteratortest_ce;

ZEPHIR_INIT_CLASS(Stub_ArrayIteratorTest);

PHP_METHOD(Stub_ArrayIteratorTest, test);

ZEPHIR_INIT_FUNCS(stub_arrayiteratortest_method_entry) {
	PHP_ME(Stub_ArrayIteratorTest, test, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
