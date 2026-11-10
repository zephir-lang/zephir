
#ifdef HAVE_CONFIG_H
#include "../ext_config.h"
#endif

#include <php.h>
#include "../php_ext.h"
#include "../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/operators.h"
#include "kernel/concat.h"
#include "kernel/array.h"
#include "kernel/string.h"


/**
 * Static property accesses run many times in one call, so every one of them
 * is answered from its cache record after the first.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2805
 */
ZEPHIR_INIT_CLASS(Stub_Issue2805)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2805, stub, issue2805, stub_issue2805_method_entry, 0);

	zend_declare_property_null(stub_issue2805_ce, SL("items"), ZEND_ACC_PUBLIC|ZEND_ACC_STATIC);
	zend_declare_property_long(stub_issue2805_ce, SL("num"), 0, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC);
	zend_declare_property_null(stub_issue2805_ce, SL("flag"), ZEND_ACC_PUBLIC|ZEND_ACC_STATIC);
	zend_declare_property_string(stub_issue2805_ce, SL("text"), "", ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);
	return SUCCESS;
}

PHP_METHOD(Stub_Issue2805, hits)
{
	zval _6$$3;
	zend_bool _18, _20, _21;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *n_param = NULL, k, _0, _1, _10, _11, *_12, _13, _14, _15, _16, _17, _19, _22, _2$$3, _3$$3, _4$$3, _5$$3, _7$$3, *_8$$3, _9$$3;
	zend_long n, i, total;

	ZVAL_UNDEF(&k);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_10);
	ZVAL_UNDEF(&_11);
	ZVAL_UNDEF(&_13);
	ZVAL_UNDEF(&_14);
	ZVAL_UNDEF(&_15);
	ZVAL_UNDEF(&_16);
	ZVAL_UNDEF(&_17);
	ZVAL_UNDEF(&_19);
	ZVAL_UNDEF(&_22);
	ZVAL_UNDEF(&_2$$3);
	ZVAL_UNDEF(&_3$$3);
	ZVAL_UNDEF(&_4$$3);
	ZVAL_UNDEF(&_5$$3);
	ZVAL_UNDEF(&_7$$3);
	ZVAL_UNDEF(&_9$$3);
	ZVAL_UNDEF(&_6$$3);
	ZEND_TLS zephir_static_property_cache _zephir_sprop_cache[19];
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &n_param);
	i = 0;
	total = 0;
	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_static_property_cached(stub_issue2805_ce, stub_issue2805_ce, SL("items"), &_zephir_sprop_cache[0], &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZVAL_UNDEF(&_1);
	ZVAL_LONG(&_1, 0);
	zephir_update_static_property_cached(stub_issue2805_ce, stub_issue2805_ce, SL("num"), &_zephir_sprop_cache[1], &_1);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZVAL_UNDEF(&_1);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_EMPTY_STRING(&_1);
	zephir_update_static_property_cached(stub_issue2805_ce, stub_issue2805_ce, SL("text"), &_zephir_sprop_cache[2], &_1);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	while (1) {
		if (!(i < n)) {
			break;
		}
		ZVAL_UNDEF(&_2$$3);
		ZVAL_LONG(&_2$$3, 2);
		zephir_add_static_property_cached(stub_issue2805_ce, stub_issue2805_ce, SL("num"), &_zephir_sprop_cache[3], &_2$$3);
		if (UNEXPECTED(EG(exception))) {
			ZEPHIR_MM_RESTORE();
			return;
		}
		ZVAL_UNDEF(&_2$$3);
		ZVAL_LONG(&_2$$3, 1);
		zephir_sub_static_property_cached(stub_issue2805_ce, stub_issue2805_ce, SL("num"), &_zephir_sprop_cache[4], &_2$$3);
		if (UNEXPECTED(EG(exception))) {
			ZEPHIR_MM_RESTORE();
			return;
		}
		zephir_read_static_property_cached(&_2$$3, stub_issue2805_ce, stub_issue2805_ce, SL("num"), &_zephir_sprop_cache[5], PH_NOISY_CC | PH_READONLY);
		if (UNEXPECTED(EG(exception))) {
			ZEPHIR_MM_RESTORE();
			return;
		}
		ZEPHIR_INIT_NVAR(&_3$$3);
		ZVAL_LONG(&_3$$3, total);
		ZEPHIR_INIT_NVAR(&_4$$3);
		zephir_add_function(&_4$$3, &_3$$3, &_2$$3);
		if (UNEXPECTED(EG(exception))) {
			ZEPHIR_MM_RESTORE();
			return;
		}
		total = zephir_get_intval(&_4$$3);
		ZEPHIR_INIT_NVAR(&_5$$3);
		ZVAL_LONG(&_5$$3, i);
		zephir_update_static_property_array_multi_cached(stub_issue2805_ce, stub_issue2805_ce, SL("items"), &_zephir_sprop_cache[6], &_5$$3, SL("a"), 1);
		if (UNEXPECTED(EG(exception))) {
			ZEPHIR_MM_RESTORE();
			return;
		}
		ZEPHIR_INIT_NVAR(&_3$$3);
		ZVAL_LONG(&_3$$3, (zephir_safe_mod_long_long(i, 3)));
		ZEPHIR_INIT_NVAR(&_6$$3);
		ZEPHIR_CONCAT_SV(&_6$$3, "k", &_3$$3);
		if (UNEXPECTED(EG(exception))) {
			ZEPHIR_MM_RESTORE();
			return;
		}
		ZEPHIR_CPY_WRT(&k, &_6$$3);
		ZEPHIR_INIT_NVAR(&_5$$3);
		ZVAL_LONG(&_5$$3, i);
		zephir_update_static_property_array_multi_cached(stub_issue2805_ce, stub_issue2805_ce, SL("items"), &_zephir_sprop_cache[7], &_5$$3, SL("z"), 1, &k);
		if (UNEXPECTED(EG(exception))) {
			ZEPHIR_MM_RESTORE();
			return;
		}
		ZEPHIR_INIT_NVAR(&_5$$3);
		ZVAL_LONG(&_5$$3, 1);
		ZEPHIR_OBS_NVAR(&_7$$3);
		_8$$3 = zephir_fetch_static_property_rw_cached(stub_issue2805_ce, stub_issue2805_ce, SL("items"), &_zephir_sprop_cache[8], &_7$$3);
		if (UNEXPECTED(EG(exception))) {
			ZEPHIR_MM_RESTORE();
			return;
		}
		zephir_array_assign_op(_8$$3, &_5$$3, add_function, SL("z"), 1, &k);
		if (UNEXPECTED(EG(exception))) {
			ZEPHIR_MM_RESTORE();
			return;
		}
		ZEPHIR_OBS_NVAR(&_9$$3);
		zephir_read_static_property_cached(&_9$$3, stub_issue2805_ce, stub_issue2805_ce, SL("text"), &_zephir_sprop_cache[9], PH_NOISY_CC);
		if (UNEXPECTED(EG(exception))) {
			ZEPHIR_MM_RESTORE();
			return;
		}
		zephir_concat_self_str(&_9$$3, SL("t"));
		if (UNEXPECTED(EG(exception))) {
			ZEPHIR_MM_RESTORE();
			return;
		}
		zephir_update_static_property_cached(stub_issue2805_ce, stub_issue2805_ce, SL("text"), &_zephir_sprop_cache[10], &_9$$3);
		if (UNEXPECTED(EG(exception))) {
			ZEPHIR_MM_RESTORE();
			return;
		}
		i++;
	}
	ZEPHIR_INIT_VAR(&_10);
	ZVAL_STRING(&_10, "k0");
	zephir_memory_observe(&_11);
	_12 = zephir_fetch_static_property_write_cached(stub_issue2805_ce, stub_issue2805_ce, SL("items"), &_zephir_sprop_cache[11], &_11);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	{ zval *zephir_unset_offsets[] = { &_10 }; zephir_array_unset_path(_12, 1, zephir_unset_offsets); }
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_create_array(return_value, 8, 0);
	zephir_memory_observe(&_13);
	zephir_read_static_property_cached(&_13, stub_issue2805_ce, stub_issue2805_ce, SL("num"), &_zephir_sprop_cache[12], PH_NOISY_CC);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_array_fast_append(return_value, &_13);
	ZEPHIR_INIT_VAR(&_14);
	ZVAL_LONG(&_14, total);
	zephir_array_fast_append(return_value, &_14);
	zephir_read_static_property_cached(&_15, stub_issue2805_ce, stub_issue2805_ce, SL("items"), &_zephir_sprop_cache[13], PH_NOISY_CC | PH_READONLY);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_INIT_NVAR(&_14);
	ZVAL_LONG(&_14, zephir_fast_count_int(&_15));
	zephir_array_fast_append(return_value, &_14);
	zephir_read_static_property_cached(&_16, stub_issue2805_ce, stub_issue2805_ce, SL("items"), &_zephir_sprop_cache[14], PH_NOISY_CC | PH_READONLY);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_OBS_NVAR(&_13);
	zephir_array_fetch_string(&_13, &_16, SL("k1"), PH_NOISY, "stub/issue2805.zep", 43);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_array_fast_append(return_value, &_13);
	zephir_read_static_property_cached(&_17, stub_issue2805_ce, stub_issue2805_ce, SL("items"), &_zephir_sprop_cache[15], PH_READONLY);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	_18 = zephir_array_isset_value_string(&_17, SL("k1"));
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_INIT_NVAR(&_14);
	ZVAL_BOOL(&_14, _18);
	zephir_array_fast_append(return_value, &_14);
	zephir_read_static_property_cached(&_19, stub_issue2805_ce, stub_issue2805_ce, SL("items"), &_zephir_sprop_cache[16], PH_READONLY);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	_20 = zephir_array_isset_value_string(&_19, SL("k0"));
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_INIT_NVAR(&_14);
	ZVAL_BOOL(&_14, _20);
	zephir_array_fast_append(return_value, &_14);
	_21 = zephir_isset_static_property_cached(stub_issue2805_ce, stub_issue2805_ce, SL("flag"), &_zephir_sprop_cache[17]);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_INIT_NVAR(&_14);
	ZVAL_BOOL(&_14, _21);
	zephir_array_fast_append(return_value, &_14);
	zephir_read_static_property_cached(&_22, stub_issue2805_ce, stub_issue2805_ce, SL("text"), &_zephir_sprop_cache[18], PH_NOISY_CC | PH_READONLY);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_INIT_NVAR(&_14);
	ZVAL_LONG(&_14, zephir_fast_strlen_ev(&_22));
	zephir_array_fast_append(return_value, &_14);
	RETURN_MM();
}

/**
 * Reads a property __get() answers, as an owned copy, as a condition and
 * as an argument, which used to leak the value __get() returned.
 */
PHP_METHOD(Stub_Issue2805, readMagic)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long n, i, c;
	zval *o, o_sub, *n_param = NULL, v, _0$$3, _1$$3;

	ZVAL_UNDEF(&o_sub);
	ZVAL_UNDEF(&v);
	ZVAL_UNDEF(&_0$$3);
	ZVAL_UNDEF(&_1$$3);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("bag", 3, 1);
	}

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(o)
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &o, &n_param);
	i = 0;
	c = 0;
	ZEPHIR_INIT_VAR(&v);
	while (1) {
		if (!(i < n)) {
			break;
		}
		ZEPHIR_OBS_NVAR(&v);
		zephir_read_property_cached(&v, o, _zephir_prop_0, 0, PH_NOISY_CC);
		ZEPHIR_OBS_NVAR(&_0$$3);
		zephir_read_property_cached(&_0$$3, o, _zephir_prop_0, 0, PH_NOISY_CC);
		if (zephir_is_true(&_0$$3)) {
			c++;
		}
		ZEPHIR_OBS_NVAR(&_1$$3);
		zephir_read_property_cached(&_1$$3, o, _zephir_prop_0, 0, PH_NOISY_CC);
		c += zephir_fast_count_int(&_1$$3);
		i++;
	}
	RETURN_MM_LONG((c + zephir_fast_count_int(&v)));
}

void zephir_init_static_properties_Stub_Issue2805()
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
		ZVAL_UNDEF(&_0);
	ZEND_TLS zephir_static_property_cache _zephir_sprop_cache[1];
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_static_property_cached(stub_issue2805_ce, stub_issue2805_ce, SL("items"), &_zephir_sprop_cache[0], &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_MM_RESTORE();
}

