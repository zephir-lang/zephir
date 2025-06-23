
#ifdef HAVE_CONFIG_H
#include "../../ext_config.h"
#endif

#include <php.h>
#include "../../php_ext.h"
#include "../../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/fcall.h"
#include "kernel/memory.h"
#include "kernel/operators.h"
#include "kernel/object.h"
#include "kernel/exception.h"
#include "ext/spl/spl_exceptions.h"
#include "kernel/concat.h"
#include "kernel/array.h"


/**
 * @issue https://github.com/zephir-lang/zephir/issues/2165
 */
ZEPHIR_INIT_CLASS(Stub_Issue2165_Issue)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Issue2165, Issue, stub, issue2165_issue, stub_issue2165_issue_method_entry, 0);

	zend_declare_property_null(stub_issue2165_issue_ce, SL("m"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(stub_issue2165_issue_ce, SL("n"), ZEND_ACC_PROTECTED);
	zend_class_implements(stub_issue2165_issue_ce, 1, stub_issue2165_issueinterface_ce);
	return SUCCESS;
}

PHP_METHOD(Stub_Issue2165_Issue, build)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *a_param = NULL, _0;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_ARRAY(a)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 0, 1, &a_param);
	if (!a_param) {
		ZEPHIR_INIT_VAR(&a);
		array_init(&a);
	} else {
		zephir_get_arrval(&a, a_param);
	}
	object_init_ex(return_value, stub_issue2165_issue_ce);
	ZVAL_BOOL(&_0, 1);
	ZEPHIR_CALL_METHOD(NULL, return_value, "__construct", NULL, 50, &a, &_0);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2165_Issue, __construct)
{
	zend_string *_6$$3;
	zend_ulong _5$$3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zephir_fcall_cache_entry *_2 = NULL, *_10 = NULL, *_13 = NULL, *_18 = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS, m, n;
	zend_bool validate, _16$$6, _22$$8, _35$$12, _40$$14;
	zval *a_param = NULL, *validate_param = NULL, i, rowA, valueA, _0, _46, _1$$3, *_3$$3, _4$$3, _7$$5, _8$$5, _9$$5, _11$$5, _12$$5, *_14$$4, _15$$4, _27$$4, _17$$6, _19$$7, _20$$7, _21$$7, _23$$8, _24$$9, _25$$9, _26$$9, _28$$11, _29$$11, _30$$11, _31$$11, _32$$11, *_33$$10, _34$$10, _45$$10, _36$$12, _37$$13, _38$$13, _39$$13, _41$$14, _42$$15, _43$$15, _44$$15;
	zval a;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&i);
	ZVAL_UNDEF(&rowA);
	ZVAL_UNDEF(&valueA);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_46);
	ZVAL_UNDEF(&_1$$3);
	ZVAL_UNDEF(&_4$$3);
	ZVAL_UNDEF(&_7$$5);
	ZVAL_UNDEF(&_8$$5);
	ZVAL_UNDEF(&_9$$5);
	ZVAL_UNDEF(&_11$$5);
	ZVAL_UNDEF(&_12$$5);
	ZVAL_UNDEF(&_15$$4);
	ZVAL_UNDEF(&_27$$4);
	ZVAL_UNDEF(&_17$$6);
	ZVAL_UNDEF(&_19$$7);
	ZVAL_UNDEF(&_20$$7);
	ZVAL_UNDEF(&_21$$7);
	ZVAL_UNDEF(&_23$$8);
	ZVAL_UNDEF(&_24$$9);
	ZVAL_UNDEF(&_25$$9);
	ZVAL_UNDEF(&_26$$9);
	ZVAL_UNDEF(&_28$$11);
	ZVAL_UNDEF(&_29$$11);
	ZVAL_UNDEF(&_30$$11);
	ZVAL_UNDEF(&_31$$11);
	ZVAL_UNDEF(&_32$$11);
	ZVAL_UNDEF(&_34$$10);
	ZVAL_UNDEF(&_45$$10);
	ZVAL_UNDEF(&_36$$12);
	ZVAL_UNDEF(&_37$$13);
	ZVAL_UNDEF(&_38$$13);
	ZVAL_UNDEF(&_39$$13);
	ZVAL_UNDEF(&_41$$14);
	ZVAL_UNDEF(&_42$$15);
	ZVAL_UNDEF(&_43$$15);
	ZVAL_UNDEF(&_44$$15);
	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_ARRAY(a)
		Z_PARAM_OPTIONAL
		Z_PARAM_BOOL(validate)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 1, &a_param, &validate_param);
	zephir_get_arrval(&a, a_param);
	if (!validate_param) {
		validate = 1;
	} else {
		}
	m = zephir_fast_count_int(&a);
	ZEPHIR_CALL_FUNCTION(&_0, "current", NULL, 45, &a);
	zephir_check_call_status();
	n = zephir_fast_count_int(&_0);
	if (validate) {
		ZEPHIR_CALL_FUNCTION(&_1$$3, "array_values", &_2, 51, &a);
		zephir_check_call_status();
		ZEPHIR_CPY_WRT(&a, &_1$$3);
		zephir_is_iterable(&a, 0, "stub/issue2165/issue.zep", 46);
		if (Z_TYPE_P(&a) == IS_ARRAY) {
			ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(&a), _5$$3, _6$$3, _3$$3)
			{
				ZEPHIR_INIT_NVAR(&i);
				if (_6$$3 != NULL) { 
					ZVAL_STR_COPY(&i, _6$$3);
				} else {
					ZVAL_LONG(&i, _5$$3);
				}
				ZEPHIR_INIT_NVAR(&rowA);
				ZVAL_COPY(&rowA, _3$$3);
				if (UNEXPECTED(zephir_fast_count_int(&rowA) != n)) {
					ZEPHIR_INIT_NVAR(&_7$$5);
					object_init_ex(&_7$$5, spl_ce_InvalidArgumentException);
					ZVAL_LONG(&_8$$5, n);
					ZEPHIR_CALL_FUNCTION(&_9$$5, "strval", &_10, 52, &_8$$5);
					zephir_check_call_status();
					ZEPHIR_INIT_NVAR(&_11$$5);
					ZVAL_LONG(&_11$$5, zephir_fast_count_int(&rowA));
					ZEPHIR_INIT_NVAR(&_12$$5);
					ZEPHIR_CONCAT_SSVSVSSVS(&_12$$5, "The number of columns", " must be equal for all rows, ", &_9$$5, " needed but ", &_11$$5, " given", " at row offset ", &i, ".");
					ZEPHIR_CALL_METHOD(NULL, &_7$$5, "__construct", &_13, 53, &_12$$5);
					zephir_check_call_status();
					zephir_throw_exception_debug(&_7$$5, "stub/issue2165/issue.zep", 33);
					ZEPHIR_MM_RESTORE();
					return;
				}
				zephir_is_iterable(&rowA, 0, "stub/issue2165/issue.zep", 44);
				if (Z_TYPE_P(&rowA) == IS_ARRAY) {
					ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(&rowA), _14$$4)
					{
						ZEPHIR_INIT_NVAR(&valueA);
						ZVAL_COPY(&valueA, _14$$4);
						_16$$6 = !(Z_TYPE_P(&valueA) == IS_LONG);
						if (_16$$6) {
							ZEPHIR_CALL_FUNCTION(&_17$$6, "is_float", &_18, 54, &valueA);
							zephir_check_call_status();
							_16$$6 = !zephir_is_true(&_17$$6);
						}
						if (UNEXPECTED(_16$$6)) {
							ZEPHIR_INIT_NVAR(&_19$$7);
							object_init_ex(&_19$$7, spl_ce_InvalidArgumentException);
							ZEPHIR_INIT_NVAR(&_20$$7);
							zephir_gettype(&_20$$7, &valueA);
							ZEPHIR_INIT_NVAR(&_21$$7);
							ZEPHIR_CONCAT_SSVS(&_21$$7, "Matrix element must", " be an integer or floating point number, ", &_20$$7, " given.");
							ZEPHIR_CALL_METHOD(NULL, &_19$$7, "__construct", &_13, 53, &_21$$7);
							zephir_check_call_status();
							zephir_throw_exception_debug(&_19$$7, "stub/issue2165/issue.zep", 40);
							ZEPHIR_MM_RESTORE();
							return;
						}
					} ZEND_HASH_FOREACH_END();
				} else {
					ZEPHIR_CALL_METHOD(NULL, &rowA, "rewind", NULL, 0);
					zephir_check_call_status();
					while (1) {
						ZEPHIR_CALL_METHOD(&_15$$4, &rowA, "valid", NULL, 0);
						zephir_check_call_status();
						if (!zend_is_true(&_15$$4)) {
							break;
						}
						ZEPHIR_CALL_METHOD(&valueA, &rowA, "current", NULL, 0);
						zephir_check_call_status();
							_22$$8 = !(Z_TYPE_P(&valueA) == IS_LONG);
							if (_22$$8) {
								ZEPHIR_CALL_FUNCTION(&_23$$8, "is_float", &_18, 54, &valueA);
								zephir_check_call_status();
								_22$$8 = !zephir_is_true(&_23$$8);
							}
							if (UNEXPECTED(_22$$8)) {
								ZEPHIR_INIT_NVAR(&_24$$9);
								object_init_ex(&_24$$9, spl_ce_InvalidArgumentException);
								ZEPHIR_INIT_NVAR(&_25$$9);
								zephir_gettype(&_25$$9, &valueA);
								ZEPHIR_INIT_NVAR(&_26$$9);
								ZEPHIR_CONCAT_SSVS(&_26$$9, "Matrix element must", " be an integer or floating point number, ", &_25$$9, " given.");
								ZEPHIR_CALL_METHOD(NULL, &_24$$9, "__construct", &_13, 53, &_26$$9);
								zephir_check_call_status();
								zephir_throw_exception_debug(&_24$$9, "stub/issue2165/issue.zep", 40);
								ZEPHIR_MM_RESTORE();
								return;
							}
						ZEPHIR_CALL_METHOD(NULL, &rowA, "next", NULL, 0);
						zephir_check_call_status();
					}
				}
				ZEPHIR_INIT_NVAR(&valueA);
				ZEPHIR_CALL_FUNCTION(&_27$$4, "array_values", &_2, 51, &rowA);
				zephir_check_call_status();
				zephir_array_append(&rowA, &_27$$4, PH_SEPARATE, "stub/issue2165/issue.zep", 44);
			} ZEND_HASH_FOREACH_END();
		} else {
			ZEPHIR_CALL_METHOD(NULL, &a, "rewind", NULL, 0);
			zephir_check_call_status();
			while (1) {
				ZEPHIR_CALL_METHOD(&_4$$3, &a, "valid", NULL, 0);
				zephir_check_call_status();
				if (!zend_is_true(&_4$$3)) {
					break;
				}
				ZEPHIR_CALL_METHOD(&i, &a, "key", NULL, 0);
				zephir_check_call_status();
				ZEPHIR_CALL_METHOD(&rowA, &a, "current", NULL, 0);
				zephir_check_call_status();
					if (UNEXPECTED(zephir_fast_count_int(&rowA) != n)) {
						ZEPHIR_INIT_NVAR(&_28$$11);
						object_init_ex(&_28$$11, spl_ce_InvalidArgumentException);
						ZVAL_LONG(&_29$$11, n);
						ZEPHIR_CALL_FUNCTION(&_30$$11, "strval", &_10, 52, &_29$$11);
						zephir_check_call_status();
						ZEPHIR_INIT_NVAR(&_31$$11);
						ZVAL_LONG(&_31$$11, zephir_fast_count_int(&rowA));
						ZEPHIR_INIT_NVAR(&_32$$11);
						ZEPHIR_CONCAT_SSVSVSSVS(&_32$$11, "The number of columns", " must be equal for all rows, ", &_30$$11, " needed but ", &_31$$11, " given", " at row offset ", &i, ".");
						ZEPHIR_CALL_METHOD(NULL, &_28$$11, "__construct", &_13, 53, &_32$$11);
						zephir_check_call_status();
						zephir_throw_exception_debug(&_28$$11, "stub/issue2165/issue.zep", 33);
						ZEPHIR_MM_RESTORE();
						return;
					}
					zephir_is_iterable(&rowA, 0, "stub/issue2165/issue.zep", 44);
					if (Z_TYPE_P(&rowA) == IS_ARRAY) {
						ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(&rowA), _33$$10)
						{
							ZEPHIR_INIT_NVAR(&valueA);
							ZVAL_COPY(&valueA, _33$$10);
							_35$$12 = !(Z_TYPE_P(&valueA) == IS_LONG);
							if (_35$$12) {
								ZEPHIR_CALL_FUNCTION(&_36$$12, "is_float", &_18, 54, &valueA);
								zephir_check_call_status();
								_35$$12 = !zephir_is_true(&_36$$12);
							}
							if (UNEXPECTED(_35$$12)) {
								ZEPHIR_INIT_NVAR(&_37$$13);
								object_init_ex(&_37$$13, spl_ce_InvalidArgumentException);
								ZEPHIR_INIT_NVAR(&_38$$13);
								zephir_gettype(&_38$$13, &valueA);
								ZEPHIR_INIT_NVAR(&_39$$13);
								ZEPHIR_CONCAT_SSVS(&_39$$13, "Matrix element must", " be an integer or floating point number, ", &_38$$13, " given.");
								ZEPHIR_CALL_METHOD(NULL, &_37$$13, "__construct", &_13, 53, &_39$$13);
								zephir_check_call_status();
								zephir_throw_exception_debug(&_37$$13, "stub/issue2165/issue.zep", 40);
								ZEPHIR_MM_RESTORE();
								return;
							}
						} ZEND_HASH_FOREACH_END();
					} else {
						ZEPHIR_CALL_METHOD(NULL, &rowA, "rewind", NULL, 0);
						zephir_check_call_status();
						while (1) {
							ZEPHIR_CALL_METHOD(&_34$$10, &rowA, "valid", NULL, 0);
							zephir_check_call_status();
							if (!zend_is_true(&_34$$10)) {
								break;
							}
							ZEPHIR_CALL_METHOD(&valueA, &rowA, "current", NULL, 0);
							zephir_check_call_status();
								_40$$14 = !(Z_TYPE_P(&valueA) == IS_LONG);
								if (_40$$14) {
									ZEPHIR_CALL_FUNCTION(&_41$$14, "is_float", &_18, 54, &valueA);
									zephir_check_call_status();
									_40$$14 = !zephir_is_true(&_41$$14);
								}
								if (UNEXPECTED(_40$$14)) {
									ZEPHIR_INIT_NVAR(&_42$$15);
									object_init_ex(&_42$$15, spl_ce_InvalidArgumentException);
									ZEPHIR_INIT_NVAR(&_43$$15);
									zephir_gettype(&_43$$15, &valueA);
									ZEPHIR_INIT_NVAR(&_44$$15);
									ZEPHIR_CONCAT_SSVS(&_44$$15, "Matrix element must", " be an integer or floating point number, ", &_43$$15, " given.");
									ZEPHIR_CALL_METHOD(NULL, &_42$$15, "__construct", &_13, 53, &_44$$15);
									zephir_check_call_status();
									zephir_throw_exception_debug(&_42$$15, "stub/issue2165/issue.zep", 40);
									ZEPHIR_MM_RESTORE();
									return;
								}
							ZEPHIR_CALL_METHOD(NULL, &rowA, "next", NULL, 0);
							zephir_check_call_status();
						}
					}
					ZEPHIR_INIT_NVAR(&valueA);
					ZEPHIR_CALL_FUNCTION(&_45$$10, "array_values", &_2, 51, &rowA);
					zephir_check_call_status();
					zephir_array_append(&rowA, &_45$$10, PH_SEPARATE, "stub/issue2165/issue.zep", 44);
				ZEPHIR_CALL_METHOD(NULL, &a, "next", NULL, 0);
				zephir_check_call_status();
			}
		}
		ZEPHIR_INIT_NVAR(&rowA);
		ZEPHIR_INIT_NVAR(&i);
	}
	zephir_update_property_zval(this_ptr, ZEND_STRL("a"), &a);
	ZVAL_UNDEF(&_46);
	ZVAL_LONG(&_46, m);
	zephir_update_property_zval(this_ptr, ZEND_STRL("m"), &_46);
	ZVAL_UNDEF(&_46);
	ZVAL_LONG(&_46, n);
	zephir_update_property_zval(this_ptr, ZEND_STRL("n"), &_46);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Issue2165_Issue, quick)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *a_param = NULL, _0;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_ARRAY(a)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 0, 1, &a_param);
	if (!a_param) {
		ZEPHIR_INIT_VAR(&a);
		array_init(&a);
	} else {
		zephir_get_arrval(&a, a_param);
	}
	object_init_ex(return_value, stub_issue2165_issue_ce);
	ZVAL_BOOL(&_0, 0);
	ZEPHIR_CALL_METHOD(NULL, return_value, "__construct", NULL, 50, &a, &_0);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2165_Issue, ones)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *m_param = NULL, *n_param = NULL, _0, _1, _2;
	zend_long m, n, ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(m)
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &m_param, &n_param);
	ZVAL_LONG(&_0, 1);
	ZVAL_LONG(&_1, m);
	ZVAL_LONG(&_2, n);
	ZEPHIR_RETURN_CALL_SELF("fill", NULL, 0, &_0, &_1, &_2);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2165_Issue, fill)
{
	zend_bool _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zephir_fcall_cache_entry *_8 = NULL, *_17 = NULL;
	zend_long m, n, ZEPHIR_LAST_CALL_STATUS;
	zval *value, value_sub, *m_param = NULL, *n_param = NULL, _1, _14, _15, _16, _18, _2$$3, _3$$3, _4$$3, _5$$4, _6$$4, _7$$4, _9$$4, _10$$5, _11$$5, _12$$5, _13$$5;

	ZVAL_UNDEF(&value_sub);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_14);
	ZVAL_UNDEF(&_15);
	ZVAL_UNDEF(&_16);
	ZVAL_UNDEF(&_18);
	ZVAL_UNDEF(&_2$$3);
	ZVAL_UNDEF(&_3$$3);
	ZVAL_UNDEF(&_4$$3);
	ZVAL_UNDEF(&_5$$4);
	ZVAL_UNDEF(&_6$$4);
	ZVAL_UNDEF(&_7$$4);
	ZVAL_UNDEF(&_9$$4);
	ZVAL_UNDEF(&_10$$5);
	ZVAL_UNDEF(&_11$$5);
	ZVAL_UNDEF(&_12$$5);
	ZVAL_UNDEF(&_13$$5);
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_ZVAL(value)
		Z_PARAM_LONG(m)
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 3, 0, &value, &m_param, &n_param);
	_0 = !(Z_TYPE_P(value) == IS_LONG);
	if (_0) {
		ZEPHIR_CALL_FUNCTION(&_1, "is_float", NULL, 54, value);
		zephir_check_call_status();
		_0 = !zephir_is_true(&_1);
	}
	if (UNEXPECTED(_0)) {
		ZEPHIR_INIT_VAR(&_2$$3);
		object_init_ex(&_2$$3, spl_ce_InvalidArgumentException);
		ZEPHIR_INIT_VAR(&_3$$3);
		zephir_gettype(&_3$$3, value);
		ZEPHIR_INIT_VAR(&_4$$3);
		ZEPHIR_CONCAT_SSVS(&_4$$3, "Value must be an", " integer or floating point number, ", &_3$$3, " given.");
		ZEPHIR_CALL_METHOD(NULL, &_2$$3, "__construct", NULL, 53, &_4$$3);
		zephir_check_call_status();
		zephir_throw_exception_debug(&_2$$3, "stub/issue2165/issue.zep", 68);
		ZEPHIR_MM_RESTORE();
		return;
	}
	if (UNEXPECTED(m < 1)) {
		ZEPHIR_INIT_VAR(&_5$$4);
		object_init_ex(&_5$$4, spl_ce_InvalidArgumentException);
		ZVAL_LONG(&_6$$4, m);
		ZEPHIR_CALL_FUNCTION(&_7$$4, "strval", &_8, 52, &_6$$4);
		zephir_check_call_status();
		ZEPHIR_INIT_VAR(&_9$$4);
		ZEPHIR_CONCAT_SSVS(&_9$$4, "M must be", " greater than 0, ", &_7$$4, " given.");
		ZEPHIR_CALL_METHOD(NULL, &_5$$4, "__construct", NULL, 53, &_9$$4);
		zephir_check_call_status();
		zephir_throw_exception_debug(&_5$$4, "stub/issue2165/issue.zep", 73);
		ZEPHIR_MM_RESTORE();
		return;
	}
	if (UNEXPECTED(n < 1)) {
		ZEPHIR_INIT_VAR(&_10$$5);
		object_init_ex(&_10$$5, spl_ce_InvalidArgumentException);
		ZVAL_LONG(&_11$$5, n);
		ZEPHIR_CALL_FUNCTION(&_12$$5, "strval", &_8, 52, &_11$$5);
		zephir_check_call_status();
		ZEPHIR_INIT_VAR(&_13$$5);
		ZEPHIR_CONCAT_SSVS(&_13$$5, "N must be", " greater than 0, ", &_12$$5, " given.");
		ZEPHIR_CALL_METHOD(NULL, &_10$$5, "__construct", NULL, 53, &_13$$5);
		zephir_check_call_status();
		zephir_throw_exception_debug(&_10$$5, "stub/issue2165/issue.zep", 78);
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZVAL_LONG(&_14, 0);
	ZVAL_LONG(&_15, n);
	ZEPHIR_CALL_FUNCTION(&_16, "array_fill", &_17, 5, &_14, &_15, value);
	zephir_check_call_status();
	ZVAL_LONG(&_14, 0);
	ZVAL_LONG(&_15, m);
	ZEPHIR_CALL_FUNCTION(&_18, "array_fill", &_17, 5, &_14, &_15, &_16);
	zephir_check_call_status();
	ZEPHIR_RETURN_CALL_SELF("quick", NULL, 0, &_18);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2165_Issue, reciprocal)
{
	zval _0, _1, _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	zephir_read_property(&_1, this_ptr, ZEND_STRL("m"), PH_NOISY_CC | PH_READONLY);
	zephir_read_property(&_2, this_ptr, ZEND_STRL("n"), PH_NOISY_CC | PH_READONLY);
	ZEPHIR_CALL_SELF(&_0, "ones", NULL, 0, &_1, &_2);
	zephir_check_call_status();
	ZEPHIR_RETURN_CALL_METHOD(&_0, "dividematrix", NULL, 0, this_ptr);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2165_Issue, divideMatrix)
{
	zval c;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *b, b_sub;

	ZVAL_UNDEF(&b_sub);
	ZVAL_UNDEF(&c);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(b, zephir_get_internal_ce(SL("stub\\issue2165\\issue")))
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &b);
	ZEPHIR_INIT_VAR(&c);
	array_init(&c);
	ZEPHIR_RETURN_CALL_SELF("quick", NULL, 0, &c);
	zephir_check_call_status();
	RETURN_MM();
}

