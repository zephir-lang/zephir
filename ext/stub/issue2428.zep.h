
extern zend_class_entry *stub_issue2428_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2428);

PHP_METHOD(Stub_Issue2428, scalarUnion);
PHP_METHOD(Stub_Issue2428, scalarUnionNullable);
PHP_METHOD(Stub_Issue2428, classUnion);
PHP_METHOD(Stub_Issue2428, classUnionNullable);
PHP_METHOD(Stub_Issue2428, classOrScalar);
PHP_METHOD(Stub_Issue2428, arrayOrObject);
PHP_METHOD(Stub_Issue2428, dynamicReturn);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_stub_issue2428_scalarunion, 0, 1, MAY_BE_LONG|MAY_BE_STRING)
	ZEND_ARG_TYPE_INFO(0, flag, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_stub_issue2428_scalarunionnullable, 0, 1, MAY_BE_NULL|MAY_BE_LONG|MAY_BE_STRING)
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_TYPE_MASK_EX(arginfo_stub_issue2428_classunion, 0, 1, stdClass|ArrayObject, 0)
	ZEND_ARG_TYPE_INFO(0, flag, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_TYPE_MASK_EX(arginfo_stub_issue2428_classunionnullable, 0, 1, stdClass|ArrayObject, MAY_BE_NULL)
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_TYPE_MASK_EX(arginfo_stub_issue2428_classorscalar, 0, 1, stdClass, MAY_BE_LONG)
	ZEND_ARG_TYPE_INFO(0, flag, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_stub_issue2428_arrayorobject, 0, 1, MAY_BE_ARRAY|MAY_BE_OBJECT)
	ZEND_ARG_TYPE_INFO(0, flag, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_stub_issue2428_dynamicreturn, 0, 1, MAY_BE_LONG|MAY_BE_STRING)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2428_method_entry) {
	PHP_ME(Stub_Issue2428, scalarUnion, arginfo_stub_issue2428_scalarunion, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2428, scalarUnionNullable, arginfo_stub_issue2428_scalarunionnullable, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2428, classUnion, arginfo_stub_issue2428_classunion, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2428, classUnionNullable, arginfo_stub_issue2428_classunionnullable, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2428, classOrScalar, arginfo_stub_issue2428_classorscalar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2428, arrayOrObject, arginfo_stub_issue2428_arrayorobject, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2428, dynamicReturn, arginfo_stub_issue2428_dynamicreturn, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
