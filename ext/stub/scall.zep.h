
extern zend_class_entry *stub_scall_ce;

ZEPHIR_INIT_CLASS(Stub_Scall);

PHP_METHOD(Stub_Scall, testMethod1);
PHP_METHOD(Stub_Scall, testMethod2);
PHP_METHOD(Stub_Scall, testMethod3);
PHP_METHOD(Stub_Scall, testMethod4);
PHP_METHOD(Stub_Scall, testMethod5);
PHP_METHOD(Stub_Scall, testMethod6);
PHP_METHOD(Stub_Scall, testMethod7);
PHP_METHOD(Stub_Scall, testCall1);
PHP_METHOD(Stub_Scall, testCall2);
PHP_METHOD(Stub_Scall, testCall3);
PHP_METHOD(Stub_Scall, testCall4);
PHP_METHOD(Stub_Scall, testCall5);
PHP_METHOD(Stub_Scall, testCall6);
PHP_METHOD(Stub_Scall, testCall7);
PHP_METHOD(Stub_Scall, testCall8);
PHP_METHOD(Stub_Scall, testCall9);
PHP_METHOD(Stub_Scall, testCall10);
PHP_METHOD(Stub_Scall, testCall11);
PHP_METHOD(Stub_Scall, testCall12);
PHP_METHOD(Stub_Scall, testCall13);
PHP_METHOD(Stub_Scall, testCall14);
PHP_METHOD(Stub_Scall, testCall15);
PHP_METHOD(Stub_Scall, testMethod16);
PHP_METHOD(Stub_Scall, testCall17);
PHP_METHOD(Stub_Scall, testCall18);
PHP_METHOD(Stub_Scall, testMethodStatic);
PHP_METHOD(Stub_Scall, interpolatedStaticReturn);
PHP_METHOD(Stub_Scall, interpolatedStaticEcho);

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_scall_testmethod1, 0, 0, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_scall_testmethod1, 0, 0, IS_STRING, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_scall_testmethod2, 0, 0, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_scall_testmethod2, 0, 0, IS_STRING, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_scall_testmethod3, 0, 0, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_scall_testmethod3, 0, 0, IS_STRING, NULL, 0)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_scall_testmethod4, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_scall_testmethod5, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_scall_testmethod6, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_scall_testmethod7, 0, 0, stdClass, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_scall_testmethod7, 0, 0, IS_OBJECT, "stdClass", 0)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_scall_testcall1, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_scall_testcall2, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_scall_testcall3, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_scall_testcall4, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_scall_testcall5, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_scall_testcall6, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_scall_testcall7, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_scall_testcall8, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_scall_testcall9, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_scall_testcall10, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_scall_testcall11, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_scall_testcall12, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_scall_testcall13, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_scall_testcall14, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_scall_testcall15, 0, 0, 0)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_scall_testmethod16, 0, 2, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_scall_testmethod16, 0, 2, IS_LONG, NULL, 0)
#endif
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, a)
#endif
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, b)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_scall_testcall17, 0, 0, 2)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, k, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, k)
#endif
	ZEND_ARG_INFO(0, p)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_scall_testcall18, 0, 0, 2)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, k, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, k)
#endif
	ZEND_ARG_INFO(0, p)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_scall_testmethodstatic, 0, 0, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_scall_testmethodstatic, 0, 0, IS_STRING, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_scall_interpolatedstaticreturn, 0, 0, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_scall_interpolatedstaticreturn, 0, 0, IS_STRING, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70100
#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_scall_interpolatedstaticecho, 0, 0, IS_VOID, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_scall_interpolatedstaticecho, 0, 0, IS_VOID, NULL, 0)
#endif
ZEND_END_ARG_INFO()
#else
#define arginfo_stub_scall_interpolatedstaticecho NULL
#endif

ZEPHIR_INIT_FUNCS(stub_scall_method_entry) {
	PHP_ME(Stub_Scall, testMethod1, arginfo_stub_scall_testmethod1, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Scall, testMethod2, arginfo_stub_scall_testmethod2, ZEND_ACC_STATIC|ZEND_ACC_PROTECTED)
	PHP_ME(Stub_Scall, testMethod3, arginfo_stub_scall_testmethod3, ZEND_ACC_STATIC|ZEND_ACC_PRIVATE)
	PHP_ME(Stub_Scall, testMethod4, arginfo_stub_scall_testmethod4, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Scall, testMethod5, arginfo_stub_scall_testmethod5, ZEND_ACC_STATIC|ZEND_ACC_PROTECTED)
	PHP_ME(Stub_Scall, testMethod6, arginfo_stub_scall_testmethod6, ZEND_ACC_STATIC|ZEND_ACC_PRIVATE)
	PHP_ME(Stub_Scall, testMethod7, arginfo_stub_scall_testmethod7, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Scall, testCall1, arginfo_stub_scall_testcall1, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Scall, testCall2, arginfo_stub_scall_testcall2, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Scall, testCall3, arginfo_stub_scall_testcall3, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Scall, testCall4, arginfo_stub_scall_testcall4, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Scall, testCall5, arginfo_stub_scall_testcall5, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Scall, testCall6, arginfo_stub_scall_testcall6, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Scall, testCall7, arginfo_stub_scall_testcall7, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Scall, testCall8, arginfo_stub_scall_testcall8, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Scall, testCall9, arginfo_stub_scall_testcall9, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Scall, testCall10, arginfo_stub_scall_testcall10, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Scall, testCall11, arginfo_stub_scall_testcall11, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Scall, testCall12, arginfo_stub_scall_testcall12, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Scall, testCall13, arginfo_stub_scall_testcall13, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Scall, testCall14, arginfo_stub_scall_testcall14, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Scall, testCall15, arginfo_stub_scall_testcall15, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Scall, testMethod16, arginfo_stub_scall_testmethod16, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Scall, testCall17, arginfo_stub_scall_testcall17, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Scall, testCall18, arginfo_stub_scall_testcall18, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Scall, testMethodStatic, arginfo_stub_scall_testmethodstatic, ZEND_ACC_STATIC|ZEND_ACC_PROTECTED)
	PHP_ME(Stub_Scall, interpolatedStaticReturn, arginfo_stub_scall_interpolatedstaticreturn, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Scall, interpolatedStaticEcho, arginfo_stub_scall_interpolatedstaticecho, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
