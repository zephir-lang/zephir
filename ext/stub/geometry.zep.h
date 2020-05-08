
extern zend_class_entry *stub_geometry_ce;

ZEPHIR_INIT_CLASS(Stub_Geometry);

PHP_METHOD(Stub_Geometry, run);
PHP_METHOD(Stub_Geometry, runOptimize);
PHP_METHOD(Stub_Geometry, distanceStatic);

#if PHP_VERSION_ID >= 70100
#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_geometry_run, 0, 2, IS_VOID, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_geometry_run, 0, 2, IS_VOID, NULL, 0)
#endif
#else
ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_geometry_run, 0, 0, 2)
#define arginfo_stub_geometry_run NULL
#endif

	ZEND_ARG_ARRAY_INFO(0, list, 0)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, count, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, count)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70100
#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_geometry_runoptimize, 0, 2, IS_VOID, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_geometry_runoptimize, 0, 2, IS_VOID, NULL, 0)
#endif
#else
ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_geometry_runoptimize, 0, 0, 2)
#define arginfo_stub_geometry_runoptimize NULL
#endif

	ZEND_ARG_ARRAY_INFO(0, list, 0)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, count, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, count)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_geometry_distancestatic, 0, 4, IS_DOUBLE, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_geometry_distancestatic, 0, 4, IS_DOUBLE, NULL, 0)
#endif
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, x1, IS_DOUBLE, 0)
#else
	ZEND_ARG_INFO(0, x1)
#endif
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, y1, IS_DOUBLE, 0)
#else
	ZEND_ARG_INFO(0, y1)
#endif
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, x2, IS_DOUBLE, 0)
#else
	ZEND_ARG_INFO(0, x2)
#endif
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, y2, IS_DOUBLE, 0)
#else
	ZEND_ARG_INFO(0, y2)
#endif
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_geometry_method_entry) {
	PHP_ME(Stub_Geometry, run, arginfo_stub_geometry_run, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Geometry, runOptimize, arginfo_stub_geometry_runoptimize, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Geometry, distanceStatic, arginfo_stub_geometry_distancestatic, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
