
extern zend_class_entry *stub_issue2654_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2654);

PHP_METHOD(Stub_Issue2654, readVar);
PHP_METHOD(Stub_Issue2654, readString);
PHP_METHOD(Stub_Issue2654, readArray);
PHP_METHOD(Stub_Issue2654, readInt);
PHP_METHOD(Stub_Issue2654, passAsArgument);
PHP_METHOD(Stub_Issue2654, captureInClosure);
PHP_METHOD(Stub_Issue2654, emptyOnUnassigned);
PHP_METHOD(Stub_Issue2654, negateUnassigned);
PHP_METHOD(Stub_Issue2654, concatUnassigned);
PHP_METHOD(Stub_Issue2654, yieldUnassigned);
PHP_METHOD(Stub_Issue2654, writeVar);
PHP_METHOD(Stub_Issue2654, optionalParam);
PHP_METHOD(Stub_Issue2654, leakProbe);
PHP_METHOD(Stub_Issue2654, leakProbeControl);
void zep_Stub_Issue2654_zephir_gen_step_yieldUnassigned(int ht, zend_execute_data *execute_data, zval *return_value, zval *this_ptr, int return_value_used, zval *zephir_gen_ext );
zend_object *zephir_init_properties_Stub_Issue2654(zend_class_entry *class_type);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2654_readvar, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2654_readstring, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2654_readarray, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2654_readint, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2654_passasargument, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2654_captureinclosure, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2654_emptyonunassigned, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2654_negateunassigned, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2654_concatunassigned, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2654_yieldunassigned, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2654_writevar, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2654_optionalparam, 0, 0, IS_STRING, 0)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2654_leakprobe, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, iterations, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2654_leakprobecontrol, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, iterations, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2654_zephir_gen_step_yieldunassigned, 0, 0, 1)
	ZEND_ARG_INFO(0, zephir_gen)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2654_zephir_init_properties_stub_issue2654, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2654_method_entry) {
PHP_ME(Stub_Issue2654, readVar, arginfo_stub_issue2654_readvar, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Issue2654, readString, arginfo_stub_issue2654_readstring, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Issue2654, readArray, arginfo_stub_issue2654_readarray, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2654, readInt, arginfo_stub_issue2654_readint, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2654, passAsArgument, arginfo_stub_issue2654_passasargument, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Issue2654, captureInClosure, arginfo_stub_issue2654_captureinclosure, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2654, emptyOnUnassigned, arginfo_stub_issue2654_emptyonunassigned, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Issue2654, negateUnassigned, arginfo_stub_issue2654_negateunassigned, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2654, concatUnassigned, arginfo_stub_issue2654_concatunassigned, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Issue2654, yieldUnassigned, arginfo_stub_issue2654_yieldunassigned, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Issue2654, writeVar, arginfo_stub_issue2654_writevar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2654, optionalParam, arginfo_stub_issue2654_optionalparam, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2654, leakProbe, arginfo_stub_issue2654_leakprobe, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2654, leakProbeControl, arginfo_stub_issue2654_leakprobecontrol, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
