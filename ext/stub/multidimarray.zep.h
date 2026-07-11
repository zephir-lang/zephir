
extern zend_class_entry *stub_multidimarray_ce;

ZEPHIR_INIT_CLASS(Stub_MultiDimArray);

PHP_METHOD(Stub_MultiDimArray, generate);
PHP_METHOD(Stub_MultiDimArray, generateTyped);
PHP_METHOD(Stub_MultiDimArray, intKeys);
PHP_METHOD(Stub_MultiDimArray, stringKeys);
PHP_METHOD(Stub_MultiDimArray, threeLevel);
PHP_METHOD(Stub_MultiDimArray, mixedKeys);
PHP_METHOD(Stub_MultiDimArray, appendFinal);
PHP_METHOD(Stub_MultiDimArray, overwriteFinal);
PHP_METHOD(Stub_MultiDimArray, overwriteArrayWithScalar);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_multidimarray_generate, 0, 1, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, size, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_multidimarray_generatetyped, 0, 1, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, size, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_multidimarray_intkeys, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_multidimarray_stringkeys, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_multidimarray_threelevel, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_multidimarray_mixedkeys, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_multidimarray_appendfinal, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_multidimarray_overwritefinal, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_multidimarray_overwritearraywithscalar, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_multidimarray_method_entry) {
	PHP_ME(Stub_MultiDimArray, generate, arginfo_stub_multidimarray_generate, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_MultiDimArray, generateTyped, arginfo_stub_multidimarray_generatetyped, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_MultiDimArray, intKeys, arginfo_stub_multidimarray_intkeys, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_MultiDimArray, stringKeys, arginfo_stub_multidimarray_stringkeys, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_MultiDimArray, threeLevel, arginfo_stub_multidimarray_threelevel, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_MultiDimArray, mixedKeys, arginfo_stub_multidimarray_mixedkeys, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_MultiDimArray, appendFinal, arginfo_stub_multidimarray_appendfinal, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_MultiDimArray, overwriteFinal, arginfo_stub_multidimarray_overwritefinal, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_MultiDimArray, overwriteArrayWithScalar, arginfo_stub_multidimarray_overwritearraywithscalar, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
