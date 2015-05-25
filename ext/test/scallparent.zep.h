
extern zend_class_entry *test_scallparent_ce;

ZEPHIR_INIT_CLASS(Test_ScallParent);

PHP_METHOD(Test_ScallParent, testMethod1);
PHP_METHOD(Test_ScallParent, testMethod2);
PHP_METHOD(Test_ScallParent, testCallStatic);
PHP_METHOD(Test_ScallParent, testMethodStatic);

ZEPHIR_INIT_FUNCS(test_scallparent_method_entry) {
	PHP_ME(Test_ScallParent, testMethod1, NULL, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Test_ScallParent, testMethod2, NULL, ZEND_ACC_STATIC|ZEND_ACC_PROTECTED)
	PHP_ME(Test_ScallParent, testCallStatic, NULL, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Test_ScallParent, testMethodStatic, NULL, ZEND_ACC_STATIC|ZEND_ACC_PROTECTED)
	PHP_FE_END
};
