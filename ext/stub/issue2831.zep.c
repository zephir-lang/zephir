
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
#include "kernel/array.h"


/**
 * Property reads as PHP's `$o->x`: `__get()` without asking `__isset()`
 * first, a warning for an undefined property or a non-object, an Error for
 * an inaccessible or uninitialized one, and a stop when the read throws.
 * `empty` and `isset` keep PHP's silent fetch.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2831
 * @see https://github.com/zephir-lang/zephir/issues/2784
 * @see https://github.com/zephir-lang/zephir/issues/2808
 */
ZEPHIR_INIT_CLASS(Stub_Issue2831)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2831, stub, issue2831, stub_issue2831_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Issue2831, read)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *o, o_sub, _0;

	ZVAL_UNDEF(&o_sub);
	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("x", 1, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(o)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &o);
	zephir_memory_observe(&_0);
	zephir_read_property_cached(&_0, o, _zephir_prop_0, 0, PH_NOISY_CC);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_Issue2831, readDynamic)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *o, o_sub, *name, name_sub, _0;

	ZVAL_UNDEF(&o_sub);
	ZVAL_UNDEF(&name_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(o)
		Z_PARAM_ZVAL(name)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &o, &name);
	zephir_memory_observe(&_0);
	zephir_read_property_zval(&_0, o, name, PH_NOISY_CC);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_Issue2831, readProtected)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *o, o_sub, _0;

	ZVAL_UNDEF(&o_sub);
	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("prot", 4, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(o)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &o);
	zephir_memory_observe(&_0);
	zephir_read_property_cached(&_0, o, _zephir_prop_0, 0, PH_NOISY_CC);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_Issue2831, readPrivate)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *o, o_sub, _0;

	ZVAL_UNDEF(&o_sub);
	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("priv", 4, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(o)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &o);
	zephir_memory_observe(&_0);
	zephir_read_property_cached(&_0, o, _zephir_prop_0, 0, PH_NOISY_CC);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_Issue2831, readTyped)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *o, o_sub, _0;

	ZVAL_UNDEF(&o_sub);
	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("typed", 5, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(o)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &o);
	zephir_memory_observe(&_0);
	zephir_read_property_cached(&_0, o, _zephir_prop_0, 0, PH_NOISY_CC);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_Issue2831, readInCondition)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *o, o_sub, _0;

	ZVAL_UNDEF(&o_sub);
	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("x", 1, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(o)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &o);
	zephir_memory_observe(&_0);
	zephir_read_property_cached(&_0, o, _zephir_prop_0, 0, PH_NOISY_CC);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	if (zephir_is_true(&_0)) {
		RETURN_MM_STRING("yes");
	}
	RETURN_MM_STRING("no");
}

PHP_METHOD(Stub_Issue2831, isEmpty)
{
	zend_bool _0;
	zval *o, o_sub;

	ZVAL_UNDEF(&o_sub);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("x", 1, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(o)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &o);
	_0 = zephir_isempty_property_fast(o, _zephir_prop_0);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	RETURN_BOOL(_0);
}

PHP_METHOD(Stub_Issue2831, isEmptyDynamic)
{
	zend_bool _0;
	zval *o, o_sub, *name, name_sub;

	ZVAL_UNDEF(&o_sub);
	ZVAL_UNDEF(&name_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(o)
		Z_PARAM_ZVAL(name)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &o, &name);
	_0 = zephir_isempty_property_zval(o, name);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	RETURN_BOOL(_0);
}

PHP_METHOD(Stub_Issue2831, issetOrEmpty)
{
	zend_bool askIsset, _0$$3, _1;
	zval *o, o_sub, *askIsset_param = NULL;

	ZVAL_UNDEF(&o_sub);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("x", 1, 1);
	}

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(o)
		Z_PARAM_BOOL(askIsset)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &o, &askIsset_param);
	if (askIsset) {
		_0$$3 = zephir_isset_property_value_fast(o, _zephir_prop_0);
		if (UNEXPECTED(EG(exception))) {
			return;
		}
		RETURN_BOOL(_0$$3);
	}
	_1 = zephir_isempty_property_fast(o, _zephir_prop_0);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	RETURN_BOOL(_1);
}

PHP_METHOD(Stub_Issue2831, issetDim)
{
	zend_bool _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *o, o_sub, _0;

	ZVAL_UNDEF(&o_sub);
	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("x", 1, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(o)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &o);
	zephir_memory_observe(&_0);
	zephir_read_property_cached(&_0, o, _zephir_prop_0, 0, 0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	_1 = zephir_array_isset_value_string(&_0, SL("k"));
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_BOOL(_1);
}

PHP_METHOD(Stub_Issue2831, issetDynamic)
{
	zend_bool _0;
	zval *o, o_sub, *name, name_sub;

	ZVAL_UNDEF(&o_sub);
	ZVAL_UNDEF(&name_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(o)
		Z_PARAM_ZVAL(name)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &o, &name);
	_0 = zephir_isset_property_value_zval(o, name);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	RETURN_BOOL(_0);
}

PHP_METHOD(Stub_Issue2831, issetNested)
{
	zend_bool _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *o, o_sub, _0;

	ZVAL_UNDEF(&o_sub);
	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	static zend_string *_zephir_prop_1 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("x", 1, 1);
	}
	if (UNEXPECTED(!_zephir_prop_1)) {
		_zephir_prop_1 = zend_string_init("y", 1, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(o)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &o);
	zephir_memory_observe(&_0);
	zephir_read_property_cached(&_0, o, _zephir_prop_0, 0, 0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	_1 = zephir_isset_property_value_fast(&_0, _zephir_prop_1);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_BOOL(_1);
}

PHP_METHOD(Stub_Issue2831, isEmptyNested)
{
	zend_bool _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *o, o_sub, _0;

	ZVAL_UNDEF(&o_sub);
	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	static zend_string *_zephir_prop_1 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("x", 1, 1);
	}
	if (UNEXPECTED(!_zephir_prop_1)) {
		_zephir_prop_1 = zend_string_init("y", 1, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(o)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &o);
	zephir_memory_observe(&_0);
	zephir_read_property_cached(&_0, o, _zephir_prop_0, 0, 0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	_1 = zephir_isempty_property_fast(&_0, _zephir_prop_1);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_BOOL(_1);
}

PHP_METHOD(Stub_Issue2831, issetThenMark)
{
	zend_bool result = 0, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *o, o_sub, *target, target_sub, __$true, __$false;

	ZVAL_UNDEF(&o_sub);
	ZVAL_UNDEF(&target_sub);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_BOOL(&__$false, 0);
	static zend_string *_zephir_prop_0 = NULL;
	static zend_string *_zephir_prop_1 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("x", 1, 1);
	}
	if (UNEXPECTED(!_zephir_prop_1)) {
		_zephir_prop_1 = zend_string_init("marked", 6, 1);
	}

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(o)
		Z_PARAM_ZVAL(target)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &o, &target);
	ZEPHIR_SEPARATE_PARAM(target);
	_0 = zephir_isset_property_value_fast(o, _zephir_prop_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	result = _0;
	if (1) {
		zephir_update_property_zval_cached(target, _zephir_prop_1, 0, &__$true);
	} else {
		zephir_update_property_zval_cached(target, _zephir_prop_1, 0, &__$false);
	}
	RETURN_MM_BOOL(result);
}

PHP_METHOD(Stub_Issue2831, isEmptyThenMark)
{
	zend_bool result = 0, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *o, o_sub, *target, target_sub, __$true, __$false;

	ZVAL_UNDEF(&o_sub);
	ZVAL_UNDEF(&target_sub);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_BOOL(&__$false, 0);
	static zend_string *_zephir_prop_0 = NULL;
	static zend_string *_zephir_prop_1 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("x", 1, 1);
	}
	if (UNEXPECTED(!_zephir_prop_1)) {
		_zephir_prop_1 = zend_string_init("marked", 6, 1);
	}

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(o)
		Z_PARAM_ZVAL(target)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &o, &target);
	ZEPHIR_SEPARATE_PARAM(target);
	_0 = zephir_isempty_property_fast(o, _zephir_prop_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	result = _0;
	if (1) {
		zephir_update_property_zval_cached(target, _zephir_prop_1, 0, &__$true);
	} else {
		zephir_update_property_zval_cached(target, _zephir_prop_1, 0, &__$false);
	}
	RETURN_MM_BOOL(result);
}

PHP_METHOD(Stub_Issue2831, addAssign)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *o, o_sub, _0, _1, _2;

	ZVAL_UNDEF(&o_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("c", 1, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(o)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &o);
	ZEPHIR_SEPARATE_PARAM(o);
	zephir_memory_observe(&_0);
	zephir_read_property_cached(&_0, o, _zephir_prop_0, 0, PH_NOISY_CC);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	ZEPHIR_INIT_VAR(&_2);
	zephir_add_function(&_2, &_0, &_1);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_update_property_zval_cached(o, _zephir_prop_0, 0, &_2);
	ZEPHIR_OBS_NVAR(&_0);
	zephir_read_property_cached(&_0, o, _zephir_prop_0, 0, PH_NOISY_CC);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_Issue2831, addAssignDynamic)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *o, o_sub, *name, name_sub, _1, _2, _3;

	ZVAL_UNDEF(&o_sub);
	ZVAL_UNDEF(&name_sub);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(o)
		Z_PARAM_ZVAL(name)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &o, &name);
	ZEPHIR_SEPARATE_PARAM(o);
	zephir_cast_to_string(&_0, name);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_memory_observe(&_1);
	zephir_read_property_zval(&_1, o, &_0, PH_NOISY_CC);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_LONG(&_2, 1);
	ZEPHIR_INIT_VAR(&_3);
	zephir_add_function(&_3, &_1, &_2);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_update_property_zval_zval(o, &_0, &_3);
	ZEPHIR_OBS_NVAR(&_1);
	zephir_read_property_zval(&_1, o, name, PH_NOISY_CC);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CCTOR(&_1);
}

PHP_METHOD(Stub_Issue2831, increment)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *o, o_sub, _0;

	ZVAL_UNDEF(&o_sub);
	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("c", 1, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(o)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &o);
	ZEPHIR_SEPARATE_PARAM(o);
	RETURN_ON_FAILURE(zephir_property_incr(o, SL("c")));
	zephir_memory_observe(&_0);
	zephir_read_property_cached(&_0, o, _zephir_prop_0, 0, PH_NOISY_CC);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_Issue2831, readThenMark)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *o, o_sub, *target, target_sub, __$true, __$false, value;

	ZVAL_UNDEF(&o_sub);
	ZVAL_UNDEF(&target_sub);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_UNDEF(&value);
	static zend_string *_zephir_prop_0 = NULL;
	static zend_string *_zephir_prop_1 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("x", 1, 1);
	}
	if (UNEXPECTED(!_zephir_prop_1)) {
		_zephir_prop_1 = zend_string_init("marked", 6, 1);
	}

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(o)
		Z_PARAM_ZVAL(target)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &o, &target);
	ZEPHIR_SEPARATE_PARAM(target);
	zephir_memory_observe(&value);
	zephir_read_property_cached(&value, o, _zephir_prop_0, 0, PH_NOISY_CC);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	if (1) {
		zephir_update_property_zval_cached(target, _zephir_prop_1, 0, &__$true);
	} else {
		zephir_update_property_zval_cached(target, _zephir_prop_1, 0, &__$false);
	}
	RETURN_CCTOR(&value);
}

PHP_METHOD(Stub_Issue2831, readProtectedThenMark)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *o, o_sub, *target, target_sub, __$true, __$false, value;

	ZVAL_UNDEF(&o_sub);
	ZVAL_UNDEF(&target_sub);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_UNDEF(&value);
	static zend_string *_zephir_prop_0 = NULL;
	static zend_string *_zephir_prop_1 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("prot", 4, 1);
	}
	if (UNEXPECTED(!_zephir_prop_1)) {
		_zephir_prop_1 = zend_string_init("marked", 6, 1);
	}

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(o)
		Z_PARAM_ZVAL(target)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &o, &target);
	ZEPHIR_SEPARATE_PARAM(target);
	zephir_memory_observe(&value);
	zephir_read_property_cached(&value, o, _zephir_prop_0, 0, PH_NOISY_CC);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	if (1) {
		zephir_update_property_zval_cached(target, _zephir_prop_1, 0, &__$true);
	} else {
		zephir_update_property_zval_cached(target, _zephir_prop_1, 0, &__$false);
	}
	RETURN_CCTOR(&value);
}

