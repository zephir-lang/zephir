
extern zend_class_entry *stub_issue2679_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2679);

PHP_METHOD(Stub_Issue2679, conditional);
PHP_METHOD(Stub_Issue2679, fetchCase);
PHP_METHOD(Stub_Issue2679, fetchGuarded);
PHP_METHOD(Stub_Issue2679, fetchStatement);
PHP_METHOD(Stub_Issue2679, exhaustiveIfElse);
PHP_METHOD(Stub_Issue2679, multiBranch);
PHP_METHOD(Stub_Issue2679, conditionalThenReassign);
PHP_METHOD(Stub_Issue2679, loopOnly);
PHP_METHOD(Stub_Issue2679, whileOnly);
PHP_METHOD(Stub_Issue2679, switchWithDefault);
PHP_METHOD(Stub_Issue2679, switchWithoutDefault);
PHP_METHOD(Stub_Issue2679, tryOnly);
PHP_METHOD(Stub_Issue2679, readBeforeWrite);
PHP_METHOD(Stub_Issue2679, stringLocal);
PHP_METHOD(Stub_Issue2679, arrayLocal);
PHP_METHOD(Stub_Issue2679, passAsArgument);
PHP_METHOD(Stub_Issue2679, conditionalInGenerator);
PHP_METHOD(Stub_Issue2679, conditionalCapturedByClosure);
PHP_METHOD(Stub_Issue2679, conditionalHeap);
PHP_METHOD(Stub_Issue2679, leakProbe);
PHP_METHOD(Stub_Issue2679, leakProbeControl);
void zep_Stub_Issue2679_zephir_gen_step_conditionalInGenerator(int ht, zend_execute_data *execute_data, zval *return_value, zval *this_ptr, int return_value_used, zval *zephir_gen_ext );
zend_object *zephir_init_properties_Stub_Issue2679(zend_class_entry *class_type);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2679_conditional, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, flag, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2679_fetchcase, 0, 1, IS_ARRAY, 0)
	ZEND_ARG_ARRAY_INFO(0, data, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2679_fetchguarded, 0, 1, IS_STRING, 0)
	ZEND_ARG_ARRAY_INFO(0, data, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2679_fetchstatement, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, data, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2679_exhaustiveifelse, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, flag, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2679_multibranch, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, a, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, b, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2679_conditionalthenreassign, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, flag, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2679_looponly, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, items, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2679_whileonly, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2679_switchwithdefault, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2679_switchwithoutdefault, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2679_tryonly, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, boom, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2679_readbeforewrite, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2679_stringlocal, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, flag, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2679_arraylocal, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, flag, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2679_passasargument, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, flag, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2679_conditionalingenerator, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, flag, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2679_conditionalcapturedbyclosure, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, flag, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2679_conditionalheap, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, flag, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2679_leakprobe, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, iterations, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2679_leakprobecontrol, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, iterations, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2679_zephir_gen_step_conditionalingenerator, 0, 0, 1)
	ZEND_ARG_INFO(0, zephir_gen)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2679_zephir_init_properties_stub_issue2679, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2679_method_entry) {
	PHP_ME(Stub_Issue2679, conditional, arginfo_stub_issue2679_conditional, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2679, fetchCase, arginfo_stub_issue2679_fetchcase, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2679, fetchGuarded, arginfo_stub_issue2679_fetchguarded, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2679, fetchStatement, arginfo_stub_issue2679_fetchstatement, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2679, exhaustiveIfElse, arginfo_stub_issue2679_exhaustiveifelse, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2679, multiBranch, arginfo_stub_issue2679_multibranch, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2679, conditionalThenReassign, arginfo_stub_issue2679_conditionalthenreassign, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2679, loopOnly, arginfo_stub_issue2679_looponly, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2679, whileOnly, arginfo_stub_issue2679_whileonly, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2679, switchWithDefault, arginfo_stub_issue2679_switchwithdefault, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2679, switchWithoutDefault, arginfo_stub_issue2679_switchwithoutdefault, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2679, tryOnly, arginfo_stub_issue2679_tryonly, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2679, readBeforeWrite, arginfo_stub_issue2679_readbeforewrite, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2679, stringLocal, arginfo_stub_issue2679_stringlocal, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2679, arrayLocal, arginfo_stub_issue2679_arraylocal, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2679, passAsArgument, arginfo_stub_issue2679_passasargument, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2679, conditionalInGenerator, arginfo_stub_issue2679_conditionalingenerator, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2679, conditionalCapturedByClosure, arginfo_stub_issue2679_conditionalcapturedbyclosure, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2679, conditionalHeap, arginfo_stub_issue2679_conditionalheap, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2679, leakProbe, arginfo_stub_issue2679_leakprobe, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2679, leakProbeControl, arginfo_stub_issue2679_leakprobecontrol, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
