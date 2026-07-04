
extern zend_class_entry *stub_generators_ce;

ZEPHIR_INIT_CLASS(Stub_Generators);

PHP_METHOD(Stub_Generators, countTo);
PHP_METHOD(Stub_Generators, keysMix);
PHP_METHOD(Stub_Generators, bare);
PHP_METHOD(Stub_Generators, withReturn);
PHP_METHOD(Stub_Generators, three);
PHP_METHOD(Stub_Generators, boom);
PHP_METHOD(Stub_Generators, seq);
PHP_METHOD(Stub_Generators, arr);
PHP_METHOD(Stub_Generators, arrReverse);
PHP_METHOD(Stub_Generators, nested);
PHP_METHOD(Stub_Generators, overItems);
PHP_METHOD(Stub_Generators, range4);
PHP_METHOD(Stub_Generators, viaSwitch);
PHP_METHOD(Stub_Generators, relay);
PHP_METHOD(Stub_Generators, consume);
void zep_Stub_Generators_zephir_gen_step_countTo(int ht, zend_execute_data *execute_data, zval *return_value, zval *this_ptr, int return_value_used, zval *zephir_gen_ext );
void zep_Stub_Generators_zephir_gen_step_keysMix(int ht, zend_execute_data *execute_data, zval *return_value, zval *this_ptr, int return_value_used, zval *zephir_gen_ext );
void zep_Stub_Generators_zephir_gen_step_bare(int ht, zend_execute_data *execute_data, zval *return_value, zval *this_ptr, int return_value_used, zval *zephir_gen_ext );
void zep_Stub_Generators_zephir_gen_step_withReturn(int ht, zend_execute_data *execute_data, zval *return_value, zval *this_ptr, int return_value_used, zval *zephir_gen_ext );
void zep_Stub_Generators_zephir_gen_step_three(int ht, zend_execute_data *execute_data, zval *return_value, zval *this_ptr, int return_value_used, zval *zephir_gen_ext );
void zep_Stub_Generators_zephir_gen_step_boom(int ht, zend_execute_data *execute_data, zval *return_value, zval *this_ptr, int return_value_used, zval *zephir_gen_ext );
void zep_Stub_Generators_zephir_gen_step_seq(int ht, zend_execute_data *execute_data, zval *return_value, zval *this_ptr, int return_value_used, zval *zephir_gen_ext );
void zep_Stub_Generators_zephir_gen_step_arr(int ht, zend_execute_data *execute_data, zval *return_value, zval *this_ptr, int return_value_used, zval *zephir_gen_ext );
void zep_Stub_Generators_zephir_gen_step_arrReverse(int ht, zend_execute_data *execute_data, zval *return_value, zval *this_ptr, int return_value_used, zval *zephir_gen_ext );
void zep_Stub_Generators_zephir_gen_step_nested(int ht, zend_execute_data *execute_data, zval *return_value, zval *this_ptr, int return_value_used, zval *zephir_gen_ext );
void zep_Stub_Generators_zephir_gen_step_overItems(int ht, zend_execute_data *execute_data, zval *return_value, zval *this_ptr, int return_value_used, zval *zephir_gen_ext );
void zep_Stub_Generators_zephir_gen_step_range4(int ht, zend_execute_data *execute_data, zval *return_value, zval *this_ptr, int return_value_used, zval *zephir_gen_ext );
void zep_Stub_Generators_zephir_gen_step_viaSwitch(int ht, zend_execute_data *execute_data, zval *return_value, zval *this_ptr, int return_value_used, zval *zephir_gen_ext );
void zep_Stub_Generators_zephir_gen_step_relay(int ht, zend_execute_data *execute_data, zval *return_value, zval *this_ptr, int return_value_used, zval *zephir_gen_ext );
zend_object *zephir_init_properties_Stub_Generators(zend_class_entry *class_type);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_generators_countto, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_generators_keysmix, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_generators_bare, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_generators_withreturn, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_generators_three, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_generators_boom, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_generators_seq, 0, 0, 4)
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, step, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, tail, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, label, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_generators_arr, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, items, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_generators_arrreverse, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, items, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_generators_nested, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, matrix, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_generators_overitems, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_generators_range4, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_generators_viaswitch, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, mode, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_generators_relay, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, it, Iterator, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_generators_consume, 0, 1, IS_ARRAY, 0)
	ZEND_ARG_OBJ_INFO(0, it, Iterator, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_generators_zephir_gen_step_countto, 0, 0, 1)
	ZEND_ARG_INFO(0, zephir_gen)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_generators_zephir_gen_step_keysmix, 0, 0, 1)
	ZEND_ARG_INFO(0, zephir_gen)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_generators_zephir_gen_step_bare, 0, 0, 1)
	ZEND_ARG_INFO(0, zephir_gen)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_generators_zephir_gen_step_withreturn, 0, 0, 1)
	ZEND_ARG_INFO(0, zephir_gen)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_generators_zephir_gen_step_three, 0, 0, 1)
	ZEND_ARG_INFO(0, zephir_gen)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_generators_zephir_gen_step_boom, 0, 0, 1)
	ZEND_ARG_INFO(0, zephir_gen)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_generators_zephir_gen_step_seq, 0, 0, 1)
	ZEND_ARG_INFO(0, zephir_gen)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_generators_zephir_gen_step_arr, 0, 0, 1)
	ZEND_ARG_INFO(0, zephir_gen)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_generators_zephir_gen_step_arrreverse, 0, 0, 1)
	ZEND_ARG_INFO(0, zephir_gen)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_generators_zephir_gen_step_nested, 0, 0, 1)
	ZEND_ARG_INFO(0, zephir_gen)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_generators_zephir_gen_step_overitems, 0, 0, 1)
	ZEND_ARG_INFO(0, zephir_gen)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_generators_zephir_gen_step_range4, 0, 0, 1)
	ZEND_ARG_INFO(0, zephir_gen)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_generators_zephir_gen_step_viaswitch, 0, 0, 1)
	ZEND_ARG_INFO(0, zephir_gen)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_generators_zephir_gen_step_relay, 0, 0, 1)
	ZEND_ARG_INFO(0, zephir_gen)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_generators_zephir_init_properties_stub_generators, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_generators_method_entry) {
	PHP_ME(Stub_Generators, countTo, arginfo_stub_generators_countto, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Generators, keysMix, arginfo_stub_generators_keysmix, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Generators, bare, arginfo_stub_generators_bare, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Generators, withReturn, arginfo_stub_generators_withreturn, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Generators, three, arginfo_stub_generators_three, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Generators, boom, arginfo_stub_generators_boom, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Generators, seq, arginfo_stub_generators_seq, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Generators, arr, arginfo_stub_generators_arr, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Generators, arrReverse, arginfo_stub_generators_arrreverse, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Generators, nested, arginfo_stub_generators_nested, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Generators, overItems, arginfo_stub_generators_overitems, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Generators, range4, arginfo_stub_generators_range4, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Generators, viaSwitch, arginfo_stub_generators_viaswitch, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Generators, relay, arginfo_stub_generators_relay, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Generators, consume, arginfo_stub_generators_consume, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
