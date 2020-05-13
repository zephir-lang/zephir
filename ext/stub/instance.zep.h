
extern zend_class_entry *stub_instance_ce;

ZEPHIR_INIT_CLASS(Stub_Instance);

PHP_METHOD(Stub_Instance, __construct);
PHP_METHOD(Stub_Instance, testIssue1339);
PHP_METHOD(Stub_Instance, testInstanceCreate);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_instance___construct, 0, 0, 11)
	ZEND_ARG_OBJ_INFO(0, a1, Stub\\Arithmetic, 0)
	ZEND_ARG_OBJ_INFO(0, a2, Stub\\ArrayObject, 0)
	ZEND_ARG_OBJ_INFO(0, a3, Stub\\Assign, 0)
	ZEND_ARG_OBJ_INFO(0, a4, Stub\\Bitwise, 0)
	ZEND_ARG_OBJ_INFO(0, a5, Stub\\BranchPrediction, 0)
	ZEND_ARG_OBJ_INFO(0, a6, Stub\\Cast, 0)
	ZEND_ARG_OBJ_INFO(0, a7, Stub\\Cblock, 0)
	ZEND_ARG_OBJ_INFO(0, a8, Stub\\Chars, 0)
	ZEND_ARG_OBJ_INFO(0, a9, Stub\\Closures, 0)
	ZEND_ARG_OBJ_INFO(0, a10, Stub\\Compare, 0)
	ZEND_ARG_OBJ_INFO(0, a11, Stub\\Concat, 0)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_instance_testissue1339, 0, 0, Stub\\Instance, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_instance_testissue1339, 0, 0, IS_OBJECT, "Stub\\Instance", 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_instance_testinstancecreate, 0, 1, Stub\\Instance, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_instance_testinstancecreate, 0, 1, IS_OBJECT, "Stub\\Instance", 0)
#endif
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, className, IS_STRING, 0)
#else
	ZEND_ARG_INFO(0, className)
#endif
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_instance_method_entry) {
	PHP_ME(Stub_Instance, __construct, arginfo_stub_instance___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Stub_Instance, testIssue1339, arginfo_stub_instance_testissue1339, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Instance, testInstanceCreate, arginfo_stub_instance_testinstancecreate, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
