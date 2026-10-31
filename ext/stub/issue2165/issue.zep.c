
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
#include "kernel/main.h"
#include "kernel/exception.h"
#include "ext/spl/spl_exceptions.h"
#include "kernel/concat.h"
#include "kernel/string.h"
#include "kernel/array.h"


/**
 * @issue https://github.com/zephir-lang/zephir/issues/2165
 */
ZEPHIR_INIT_CLASS(Stub_Issue2165_Issue)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Issue2165, Issue, stub, issue2165_issue, stub_issue2165_issue_method_entry, 0);

	zend_declare_property_null(stub_issue2165_issue_ce, SL("a"), ZEND_ACC_PROTECTED);
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
	ZEND_TLS zephir_method_cache _zephir_call_cache[1];
	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
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
	ZEPHIR_CALL_METHOD_CACHED(NULL, return_value, "__construct", &_zephir_call_cache[0], &a, &_0);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2165_Issue, __construct)
{
	zend_string *_5$$3;
	zend_ulong _4$$3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zephir_fcall_cache_entry *_2 = NULL, *_9 = NULL, *_17 = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS, m, n;
	zend_bool validate, _22$$4, _15$$6, _23$$8;
	zval *a_param = NULL, *validate_param = NULL, i, rowA, valueA, _0, _29, _1$$3, *_3$$3, _6$$5, _7$$5, _8$$5, _10$$5, _11$$5, *_12$$4, _13$$4, *_14$$4, _21$$4, _28$$4, _16$$6, _18$$7, _19$$7, _20$$7, _24$$8, _25$$9, _26$$9, _27$$9;
	zval a;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&i);
	ZVAL_UNDEF(&rowA);
	ZVAL_UNDEF(&valueA);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_29);
	ZVAL_UNDEF(&_1$$3);
	ZVAL_UNDEF(&_6$$5);
	ZVAL_UNDEF(&_7$$5);
	ZVAL_UNDEF(&_8$$5);
	ZVAL_UNDEF(&_10$$5);
	ZVAL_UNDEF(&_11$$5);
	ZVAL_UNDEF(&_13$$4);
	ZVAL_UNDEF(&_21$$4);
	ZVAL_UNDEF(&_28$$4);
	ZVAL_UNDEF(&_16$$6);
	ZVAL_UNDEF(&_18$$7);
	ZVAL_UNDEF(&_19$$7);
	ZVAL_UNDEF(&_20$$7);
	ZVAL_UNDEF(&_24$$8);
	ZVAL_UNDEF(&_25$$9);
	ZVAL_UNDEF(&_26$$9);
	ZVAL_UNDEF(&_27$$9);
	ZEND_TLS zephir_method_cache _zephir_call_cache[7];
	static zend_string *_zephir_prop_0 = NULL;
	static zend_string *_zephir_prop_1 = NULL;
	static zend_string *_zephir_prop_2 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("a", 1, 1);
	}
	if (UNEXPECTED(!_zephir_prop_1)) {
		_zephir_prop_1 = zend_string_init("m", 1, 1);
	}
	if (UNEXPECTED(!_zephir_prop_2)) {
		_zephir_prop_2 = zend_string_init("n", 1, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 2)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
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
	ZEPHIR_CALL_FUNCTION(&_0, "current", NULL, 39, &a);
	zephir_check_call_status();
	n = zephir_fast_count_int(&_0);
	if (validate) {
		ZEPHIR_CALL_FUNCTION(&_1$$3, "array_values", &_2, 41, &a);
		zephir_check_call_status();
		if (UNEXPECTED(Z_TYPE_P(&_1$$3) != IS_ARRAY)) {
			zephir_throw_variable_type_error(&_1$$3, "a", "array");
			ZEPHIR_MM_RESTORE();
			return;
		}
		ZEPHIR_CPY_WRT(&a, &_1$$3);
		zephir_is_iterable(&a, 0, "stub/issue2165/issue.zep", 47);
		ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(&a), _4$$3, _5$$3, _3$$3)
		{
			ZEPHIR_INIT_NVAR(&i);
			if (_5$$3 != NULL) { 
				ZVAL_STR_COPY(&i, _5$$3);
			} else {
				ZVAL_LONG(&i, _4$$3);
			}
			ZEPHIR_INIT_NVAR(&rowA);
			ZVAL_COPY(&rowA, _3$$3);
			if (UNEXPECTED(zephir_fast_count_int(&rowA) != n)) {
				ZEPHIR_INIT_NVAR(&_6$$5);
				object_init_ex(&_6$$5, spl_ce_InvalidArgumentException);
				ZVAL_LONG(&_7$$5, n);
				ZEPHIR_CALL_FUNCTION(&_8$$5, "strval", &_9, 42, &_7$$5);
				zephir_check_call_status();
				ZEPHIR_INIT_NVAR(&_10$$5);
				ZVAL_LONG(&_10$$5, zephir_fast_count_int(&rowA));
				ZEPHIR_INIT_NVAR(&_11$$5);
				ZEPHIR_CONCAT_SSVSVSSVS(&_11$$5, "The number of columns", " must be equal for all rows, ", &_8$$5, " needed but ", &_10$$5, " given", " at row offset ", &i, ".");
				if (UNEXPECTED(EG(exception))) {
					ZEPHIR_MM_RESTORE();
					return;
				}
				ZEPHIR_CALL_METHOD_CACHED(NULL, &_6$$5, "__construct", &_zephir_call_cache[0], &_11$$5);
				zephir_check_call_status();
				zephir_throw_exception_debug(&_6$$5, "stub/issue2165/issue.zep", 34);
				ZEPHIR_MM_RESTORE();
				return;
			}
			if (Z_TYPE_P(&rowA) == IS_STRING) {
				ZEPHIR_INIT_NVAR(&_13$$4);
				zephir_string_to_char_array(&_13$$4, &rowA);
				_12$$4 = &_13$$4;
			} else {
				_12$$4 = &rowA;
			}
			zephir_is_iterable(_12$$4, 0, "stub/issue2165/issue.zep", 45);
			if (Z_TYPE_P(_12$$4) == IS_ARRAY) {
				ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(_12$$4), _14$$4)
				{
					ZEPHIR_INIT_NVAR(&valueA);
					ZVAL_COPY(&valueA, _14$$4);
					_15$$6 = !((Z_TYPE_P(&valueA) == IS_LONG));
					if (_15$$6) {
						ZEPHIR_CALL_FUNCTION(&_16$$6, "is_float", &_17, 43, &valueA);
						zephir_check_call_status();
						_15$$6 = !zephir_is_true(&_16$$6);
					}
					if (UNEXPECTED(_15$$6)) {
						ZEPHIR_INIT_NVAR(&_18$$7);
						object_init_ex(&_18$$7, spl_ce_InvalidArgumentException);
						ZEPHIR_INIT_NVAR(&_19$$7);
						zephir_gettype(&_19$$7, &valueA);
						ZEPHIR_INIT_NVAR(&_20$$7);
						ZEPHIR_CONCAT_SSVS(&_20$$7, "Matrix element must", " be an integer or floating point number, ", &_19$$7, " given.");
						if (UNEXPECTED(EG(exception))) {
							ZEPHIR_MM_RESTORE();
							return;
						}
						ZEPHIR_CALL_METHOD_CACHED(NULL, &_18$$7, "__construct", &_zephir_call_cache[1], &_20$$7);
						zephir_check_call_status();
						zephir_throw_exception_debug(&_18$$7, "stub/issue2165/issue.zep", 41);
						ZEPHIR_MM_RESTORE();
						return;
					}
				} ZEND_HASH_FOREACH_END();
			} else {
				ZEPHIR_CALL_METHOD_CACHED(NULL, _12$$4, "rewind", &_zephir_call_cache[2]);
				zephir_check_call_status();
				_22$$4 = 1;
				while (1) {
					if (_22$$4) {
						_22$$4 = 0;
					} else {
						ZEPHIR_CALL_METHOD_CACHED(NULL, _12$$4, "next", &_zephir_call_cache[3]);
						zephir_check_call_status();
					}
					ZEPHIR_CALL_METHOD_CACHED(&_21$$4, _12$$4, "valid", &_zephir_call_cache[4]);
					zephir_check_call_status();
					if (!zend_is_true(&_21$$4)) {
						break;
					}
					ZEPHIR_CALL_METHOD_CACHED(&valueA, _12$$4, "current", &_zephir_call_cache[5]);
					zephir_check_call_status();
						_23$$8 = !((Z_TYPE_P(&valueA) == IS_LONG));
						if (_23$$8) {
							ZEPHIR_CALL_FUNCTION(&_24$$8, "is_float", &_17, 43, &valueA);
							zephir_check_call_status();
							_23$$8 = !zephir_is_true(&_24$$8);
						}
						if (UNEXPECTED(_23$$8)) {
							ZEPHIR_INIT_NVAR(&_25$$9);
							object_init_ex(&_25$$9, spl_ce_InvalidArgumentException);
							ZEPHIR_INIT_NVAR(&_26$$9);
							zephir_gettype(&_26$$9, &valueA);
							ZEPHIR_INIT_NVAR(&_27$$9);
							ZEPHIR_CONCAT_SSVS(&_27$$9, "Matrix element must", " be an integer or floating point number, ", &_26$$9, " given.");
							if (UNEXPECTED(EG(exception))) {
								ZEPHIR_MM_RESTORE();
								return;
							}
							ZEPHIR_CALL_METHOD_CACHED(NULL, &_25$$9, "__construct", &_zephir_call_cache[6], &_27$$9);
							zephir_check_call_status();
							zephir_throw_exception_debug(&_25$$9, "stub/issue2165/issue.zep", 41);
							ZEPHIR_MM_RESTORE();
							return;
						}
				}
			}
			ZEPHIR_INIT_NVAR(&valueA);
			ZEPHIR_CALL_FUNCTION(&_28$$4, "array_values", &_2, 41, &rowA);
			zephir_check_call_status();
			zephir_array_append(&rowA, &_28$$4, PH_SEPARATE, "stub/issue2165/issue.zep", 45);
			if (UNEXPECTED(EG(exception))) {
				ZEPHIR_MM_RESTORE();
				return;
			}
		} ZEND_HASH_FOREACH_END();
		ZEPHIR_INIT_NVAR(&rowA);
		ZEPHIR_INIT_NVAR(&i);
	}
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 56, &a);
	ZVAL_UNDEF(&_29);
	ZVAL_LONG(&_29, m);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_1, 57, &_29);
	ZVAL_UNDEF(&_29);
	ZVAL_LONG(&_29, n);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_2, 58, &_29);
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
	ZEND_TLS zephir_method_cache _zephir_call_cache[1];
	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
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
	ZEPHIR_CALL_METHOD_CACHED(NULL, return_value, "__construct", &_zephir_call_cache[0], &a, &_0);
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
	ZEND_TLS zephir_method_cache _zephir_call_cache[1];
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
	ZEPHIR_RETURN_CALL_SELF_CACHED("fill", &_zephir_call_cache[0], &_0, &_1, &_2);
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
	ZEND_TLS zephir_method_cache _zephir_call_cache[4];
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_ZVAL(value)
		Z_PARAM_LONG(m)
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 3, 0, &value, &m_param, &n_param);
	_0 = !((Z_TYPE_P(value) == IS_LONG));
	if (_0) {
		ZEPHIR_CALL_FUNCTION(&_1, "is_float", NULL, 43, value);
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
		if (UNEXPECTED(EG(exception))) {
			ZEPHIR_MM_RESTORE();
			return;
		}
		ZEPHIR_CALL_METHOD_CACHED(NULL, &_2$$3, "__construct", &_zephir_call_cache[0], &_4$$3);
		zephir_check_call_status();
		zephir_throw_exception_debug(&_2$$3, "stub/issue2165/issue.zep", 69);
		ZEPHIR_MM_RESTORE();
		return;
	}
	if (UNEXPECTED(m < 1)) {
		ZEPHIR_INIT_VAR(&_5$$4);
		object_init_ex(&_5$$4, spl_ce_InvalidArgumentException);
		ZVAL_LONG(&_6$$4, m);
		ZEPHIR_CALL_FUNCTION(&_7$$4, "strval", &_8, 42, &_6$$4);
		zephir_check_call_status();
		ZEPHIR_INIT_VAR(&_9$$4);
		ZEPHIR_CONCAT_SSVS(&_9$$4, "M must be", " greater than 0, ", &_7$$4, " given.");
		if (UNEXPECTED(EG(exception))) {
			ZEPHIR_MM_RESTORE();
			return;
		}
		ZEPHIR_CALL_METHOD_CACHED(NULL, &_5$$4, "__construct", &_zephir_call_cache[1], &_9$$4);
		zephir_check_call_status();
		zephir_throw_exception_debug(&_5$$4, "stub/issue2165/issue.zep", 74);
		ZEPHIR_MM_RESTORE();
		return;
	}
	if (UNEXPECTED(n < 1)) {
		ZEPHIR_INIT_VAR(&_10$$5);
		object_init_ex(&_10$$5, spl_ce_InvalidArgumentException);
		ZVAL_LONG(&_11$$5, n);
		ZEPHIR_CALL_FUNCTION(&_12$$5, "strval", &_8, 42, &_11$$5);
		zephir_check_call_status();
		ZEPHIR_INIT_VAR(&_13$$5);
		ZEPHIR_CONCAT_SSVS(&_13$$5, "N must be", " greater than 0, ", &_12$$5, " given.");
		if (UNEXPECTED(EG(exception))) {
			ZEPHIR_MM_RESTORE();
			return;
		}
		ZEPHIR_CALL_METHOD_CACHED(NULL, &_10$$5, "__construct", &_zephir_call_cache[2], &_13$$5);
		zephir_check_call_status();
		zephir_throw_exception_debug(&_10$$5, "stub/issue2165/issue.zep", 79);
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZVAL_LONG(&_14, 0);
	ZVAL_LONG(&_15, n);
	ZEPHIR_CALL_FUNCTION(&_16, "array_fill", &_17, 4, &_14, &_15, value);
	zephir_check_call_status();
	ZVAL_LONG(&_14, 0);
	ZVAL_LONG(&_15, m);
	ZEPHIR_CALL_FUNCTION(&_18, "array_fill", &_17, 4, &_14, &_15, &_16);
	zephir_check_call_status();
	ZEPHIR_RETURN_CALL_SELF_CACHED("quick", &_zephir_call_cache[3], &_18);
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
	ZEND_TLS zephir_method_cache _zephir_call_cache[2];
	static zend_string *_zephir_prop_0 = NULL;
	static zend_string *_zephir_prop_1 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("m", 1, 1);
	}
	if (UNEXPECTED(!_zephir_prop_1)) {
		_zephir_prop_1 = zend_string_init("n", 1, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	zephir_read_property_cached(&_1, this_ptr, _zephir_prop_0, 57, PH_NOISY_CC | PH_READONLY);
	zephir_read_property_cached(&_2, this_ptr, _zephir_prop_1, 58, PH_NOISY_CC | PH_READONLY);
	ZEPHIR_CALL_SELF_CACHED(&_0, "ones", &_zephir_call_cache[0], &_1, &_2);
	zephir_check_call_status();
	ZEPHIR_RETURN_CALL_METHOD_CACHED(&_0, "divideMatrix", &_zephir_call_cache[1], this_ptr);
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
	ZEND_TLS zephir_method_cache _zephir_call_cache[1];
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(b, stub_issue2165_issue_ce)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &b);
	ZEPHIR_INIT_VAR(&c);
	array_init(&c);
	ZEPHIR_RETURN_CALL_SELF_CACHED("quick", &_zephir_call_cache[0], &c);
	zephir_check_call_status();
	RETURN_MM();
}

