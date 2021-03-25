
extern zend_class_entry *stub_geometry_ce;

ZEPHIR_INIT_CLASS(Stub_Geometry);

PHP_METHOD(Stub_Geometry, run);
PHP_METHOD(Stub_Geometry, runOptimize);
PHP_METHOD(Stub_Geometry, distanceStatic);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_geometry_run, 0, 2, IS_VOID, 0)

	ZEND_ARG_ARRAY_INFO(0, list, 0)
	ZEND_ARG_TYPE_INFO(0, count, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_geometry_runoptimize, 0, 2, IS_VOID, 0)

	ZEND_ARG_ARRAY_INFO(0, list, 0)
	ZEND_ARG_TYPE_INFO(0, count, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_geometry_distancestatic, 0, 4, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, x1, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, y1, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, x2, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, y2, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_geometry_method_entry) {
	PHP_ME(Stub_Geometry, run, arginfo_stub_geometry_run, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Geometry, runOptimize, arginfo_stub_geometry_runoptimize, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Geometry, distanceStatic, arginfo_stub_geometry_distancestatic, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
