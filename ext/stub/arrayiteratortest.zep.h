
extern zend_class_entry *stub_arrayiteratortest_ce;

ZEPHIR_INIT_CLASS(Stub_ArrayIteratorTest);

PHP_METHOD(Stub_ArrayIteratorTest, test);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_arrayiteratortest_test, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_arrayiteratortest_method_entry) {
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_ArrayIteratorTest, test, arginfo_stub_arrayiteratortest_test, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
#else
	PHP_ME(Stub_ArrayIteratorTest, test, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
#endif
	PHP_FE_END
};
