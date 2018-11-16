
extern zend_class_entry *test_scallparent_ce;

ZEPHIR_INIT_CLASS(Test_ScallParent);

PHP_METHOD(Test_ScallParent, testMethod1);
PHP_METHOD(Test_ScallParent, testMethod2);
PHP_METHOD(Test_ScallParent, testCallStatic);
PHP_METHOD(Test_ScallParent, testMethodStatic);

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_scallparent_testmethod1, 0, 0, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_scallparent_testmethod1, 0, 0, IS_STRING, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_scallparent_testmethod2, 0, 0, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_scallparent_testmethod2, 0, 0, IS_STRING, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_scallparent_testcallstatic, 0, 0, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_scallparent_testcallstatic, 0, 0, IS_STRING, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_scallparent_testmethodstatic, 0, 0, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_scallparent_testmethodstatic, 0, 0, IS_STRING, NULL, 0)
#endif
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_scallparent_method_entry) {
	PHP_ME(Test_ScallParent, testMethod1, arginfo_test_scallparent_testmethod1, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Test_ScallParent, testMethod2, arginfo_test_scallparent_testmethod2, ZEND_ACC_STATIC|ZEND_ACC_PROTECTED)
	PHP_ME(Test_ScallParent, testCallStatic, arginfo_test_scallparent_testcallstatic, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Test_ScallParent, testMethodStatic, arginfo_test_scallparent_testmethodstatic, ZEND_ACC_STATIC|ZEND_ACC_PROTECTED)
	PHP_FE_END
};
