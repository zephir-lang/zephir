
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
#include "kernel/object.h"
#include "kernel/memory.h"
#include "kernel/operators.h"
#include "kernel/array.h"


/**
 * Exercises the cached object-property read/write path
 * (zephir_read_property_cached / zephir_update_property_zval_cached).
 * Backs PropertyCacheTest.
 *
 * @see https://github.com/zephir-lang/zephir/issues/1884 (property access)
 */
ZEPHIR_INIT_CLASS(Stub_PropertyCache)
{
	ZEPHIR_REGISTER_CLASS(Stub, PropertyCache, stub, propertycache, stub_propertycache_method_entry, 0);

	zend_declare_property_long(stub_propertycache_ce, SL("pub"), 1, ZEND_ACC_PUBLIC);
	zend_declare_property_long(stub_propertycache_ce, SL("prot"), 2, ZEND_ACC_PROTECTED);
	zend_declare_property_long(stub_propertycache_ce, SL("priv"), 3, ZEND_ACC_PRIVATE);
	return SUCCESS;
}

PHP_METHOD(Stub_PropertyCache, getPub)
{

	RETURN_MEMBER(getThis(), "pub");
}

PHP_METHOD(Stub_PropertyCache, setPub)
{
	zval *value, value_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&value_sub);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("pub", 3, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &value);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 5, value);
}

PHP_METHOD(Stub_PropertyCache, getProt)
{

	RETURN_MEMBER(getThis(), "prot");
}

PHP_METHOD(Stub_PropertyCache, setProt)
{
	zval *value, value_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&value_sub);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("prot", 4, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &value);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 6, value);
}

PHP_METHOD(Stub_PropertyCache, getPriv)
{

	RETURN_MEMBER(getThis(), "priv");
}

PHP_METHOD(Stub_PropertyCache, setPriv)
{
	zval *value, value_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&value_sub);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("priv", 4, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &value);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 7, value);
}

/**
 * Compound read-modify-write on a property (read + write emitters).
 */
PHP_METHOD(Stub_PropertyCache, incPub)
{
	zval _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("pub", 3, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 5, PH_NOISY_CC);
	ZEPHIR_ADD_ASSIGN(&_0, &_1)
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 5, &_0);
	RETURN_MM_MEMBER(getThis(), "pub");
}

/**
 * Same property read at two sites in one method: first is the cache
 * miss/fill, second is the cache hit (Stage 2).
 */
PHP_METHOD(Stub_PropertyCache, sumPubTwice)
{
	zval _0, _1;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("pub", 3, 1);
	}
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 5, PH_NOISY_CC | PH_READONLY);
	zephir_read_property_cached(&_1, this_ptr, _zephir_prop_0, 5, PH_NOISY_CC | PH_READONLY);
	zephir_add_function(return_value, &_0, &_1);
	return;
}

/**
 * Value-type transition at one write site: array then scalar.
 */
PHP_METHOD(Stub_PropertyCache, swapArrayScalar)
{
	zval _1, _2;
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("pub", 3, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 3, 0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 2);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 3);
	zephir_array_fast_append(&_0, &_1);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 5, &_0);
	ZVAL_UNDEF(&_2);
	ZVAL_LONG(&_2, 42);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 5, &_2);
	RETURN_MM_MEMBER(getThis(), "pub");
}

/**
 * Reads a property in both arms of a branch and after it: the name slot
 * must be declared at function scope so every arm can reference it.
 */
PHP_METHOD(Stub_PropertyCache, branchedRead)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *flag_param = NULL, out, _0;
	zend_bool flag;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&out);
	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	static zend_string *_zephir_prop_1 = NULL;
	static zend_string *_zephir_prop_2 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("pub", 3, 1);
	}
	if (UNEXPECTED(!_zephir_prop_1)) {
		_zephir_prop_1 = zend_string_init("prot", 4, 1);
	}
	if (UNEXPECTED(!_zephir_prop_2)) {
		_zephir_prop_2 = zend_string_init("priv", 4, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_BOOL(flag)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &flag_param);
	ZEPHIR_INIT_VAR(&out);
	ZVAL_LONG(&out, 0);
	if (flag) {
		ZEPHIR_OBS_NVAR(&out);
		zephir_read_property_cached(&out, this_ptr, _zephir_prop_0, 5, PH_NOISY_CC);
	} else {
		ZEPHIR_OBS_NVAR(&out);
		zephir_read_property_cached(&out, this_ptr, _zephir_prop_1, 6, PH_NOISY_CC);
	}
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_2, 7, PH_NOISY_CC | PH_READONLY);
	zephir_add_function(return_value, &out, &_0);
	RETURN_MM();
}

/**
 * Tight loop over a property read (cache-hit path under load).
 */
PHP_METHOD(Stub_PropertyCache, readLoop)
{
	zend_bool _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *n_param = NULL, total, i, _3$$3;
	zend_long n, _1, _2;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&total);
	ZVAL_UNDEF(&i);
	ZVAL_UNDEF(&_3$$3);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("pub", 3, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &n_param);
	ZEPHIR_INIT_VAR(&total);
	ZVAL_LONG(&total, 0);
	ZEPHIR_INIT_VAR(&i);
	ZVAL_LONG(&i, 0);
	_2 = n;
	_1 = 1;
	_0 = 0;
	if (_1 <= _2) {
		while (1) {
			if (_0) {
				_1++;
				if (!(_1 <= _2)) {
					break;
				}
			} else {
				_0 = 1;
			}
			ZEPHIR_INIT_NVAR(&i);
			ZVAL_LONG(&i, _1);
			zephir_read_property_cached(&_3$$3, this_ptr, _zephir_prop_0, 5, PH_NOISY_CC | PH_READONLY);
			ZEPHIR_ADD_ASSIGN(&total, &_3$$3);
		}
	}
	RETURN_CCTOR(&total);
}

/**
 * Property value CHANGES on every iteration: a cached write then a cached
 * read of the same property in the SAME iteration. The slot caches the
 * offset, never the value, so the read must observe the just-written value
 * each time. A stale cached value would break the running total.
 * Returns 1 + 2 + ... + n.
 */
PHP_METHOD(Stub_PropertyCache, mutateInLoop)
{
	zend_bool _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *n_param = NULL, i, total, _3$$3;
	zend_long n, _1, _2;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&i);
	ZVAL_UNDEF(&total);
	ZVAL_UNDEF(&_3$$3);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("pub", 3, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &n_param);
	ZEPHIR_INIT_VAR(&i);
	ZVAL_LONG(&i, 0);
	ZEPHIR_INIT_VAR(&total);
	ZVAL_LONG(&total, 0);
	_2 = n;
	_1 = 1;
	_0 = 0;
	if (_1 <= _2) {
		while (1) {
			if (_0) {
				_1++;
				if (!(_1 <= _2)) {
					break;
				}
			} else {
				_0 = 1;
			}
			ZEPHIR_INIT_NVAR(&i);
			ZVAL_LONG(&i, _1);
			zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 5, &i);
			zephir_read_property_cached(&_3$$3, this_ptr, _zephir_prop_0, 5, PH_NOISY_CC | PH_READONLY);
			ZEPHIR_ADD_ASSIGN(&total, &_3$$3);
		}
	}
	RETURN_CCTOR(&total);
}

/**
 * Compound read-modify-write of one property inside a loop: each iteration
 * emits BOTH a cached read and a cached write against the same slot while
 * the value keeps changing. Returns the final value (1 + 2 + ... + n).
 */
PHP_METHOD(Stub_PropertyCache, accumulateInLoop)
{
	zend_bool _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *n_param = NULL, i, _0, _4$$3;
	zend_long n, _2, _3;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&i);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_4$$3);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("pub", 3, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &n_param);
	ZEPHIR_INIT_VAR(&i);
	ZVAL_LONG(&i, 0);
	ZVAL_UNDEF(&_0);
	ZVAL_LONG(&_0, 0);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 5, &_0);
	_3 = n;
	_2 = 1;
	_1 = 0;
	if (_2 <= _3) {
		while (1) {
			if (_1) {
				_2++;
				if (!(_2 <= _3)) {
					break;
				}
			} else {
				_1 = 1;
			}
			ZEPHIR_INIT_NVAR(&i);
			ZVAL_LONG(&i, _2);
			zephir_read_property_cached(&_4$$3, this_ptr, _zephir_prop_0, 5, PH_NOISY_CC);
			ZEPHIR_ADD_ASSIGN(&_4$$3, &i)
			zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 5, &_4$$3);
		}
	}
	RETURN_MM_MEMBER(getThis(), "pub");
}

