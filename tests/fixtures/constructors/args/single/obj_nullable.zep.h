
extern zend_class_entry *stub_args_single_objnullable_ce;

ZEPHIR_INIT_CLASS(Stub_Args_Single_ObjNullable);

PHP_METHOD(Stub_Args_Single_ObjNullable, argObjNull);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_args_single_objnullable_argobjnull, 0, 0, IS_VOID, 0)

	ZEND_ARG_OBJ_TYPE_MASK(0, param, Stub\\Args\\Single\\ObjNullable, MAY_BE_NULL, "null")
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_args_single_objnullable_method_entry) {
	PHP_ME(Stub_Args_Single_ObjNullable, argObjNull, arginfo_stub_args_single_objnullable_argobjnull, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
