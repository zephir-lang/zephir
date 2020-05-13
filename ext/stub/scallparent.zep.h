
extern zend_class_entry *stub_scallparent_ce;

ZEPHIR_INIT_CLASS(Stub_ScallParent);

PHP_METHOD(Stub_ScallParent, testMethod1);
PHP_METHOD(Stub_ScallParent, testMethod2);
PHP_METHOD(Stub_ScallParent, testCallStatic);
PHP_METHOD(Stub_ScallParent, testMethodStatic);

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_scallparent_testmethod1, 0, 0, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_scallparent_testmethod1, 0, 0, IS_STRING, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_scallparent_testmethod2, 0, 0, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_scallparent_testmethod2, 0, 0, IS_STRING, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_scallparent_testcallstatic, 0, 0, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_scallparent_testcallstatic, 0, 0, IS_STRING, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_scallparent_testmethodstatic, 0, 0, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_scallparent_testmethodstatic, 0, 0, IS_STRING, NULL, 0)
#endif
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_scallparent_method_entry) {
	PHP_ME(Stub_ScallParent, testMethod1, arginfo_stub_scallparent_testmethod1, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ScallParent, testMethod2, arginfo_stub_scallparent_testmethod2, ZEND_ACC_STATIC|ZEND_ACC_PROTECTED)
	PHP_ME(Stub_ScallParent, testCallStatic, arginfo_stub_scallparent_testcallstatic, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ScallParent, testMethodStatic, arginfo_stub_scallparent_testmethodstatic, ZEND_ACC_STATIC|ZEND_ACC_PROTECTED)
	PHP_FE_END
};
