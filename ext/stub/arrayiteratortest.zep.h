
extern zend_class_entry *stub_arrayiteratortest_ce;

ZEPHIR_INIT_CLASS(Stub_ArrayIteratorTest);

PHP_METHOD(Stub_ArrayIteratorTest, test);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_arrayiteratortest_test, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_arrayiteratortest_method_entry) {
PHP_ME(Stub_ArrayIteratorTest, test, arginfo_stub_arrayiteratortest_test, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
