
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
#include "kernel/array.h"
#include "kernel/operators.h"
#include "kernel/object.h"


/**
 * Multi-dimensional array assignment (`let a[i][j] = v`).
 *
 * Exercises the offset shapes handled by zephir_array_update_multi_ex:
 * dynamic keys ("z"), literal long keys ("l"), literal string keys ("s"),
 * mixed chains, append-final ("a"), and last-level overwrite. Backs the
 * regression guard for issue #1884 (final-offset fast-path in the kernel).
 *
 * @see https://github.com/zephir-lang/zephir/issues/1884
 */
ZEPHIR_INIT_CLASS(Stub_MultiDimArray)
{
	ZEPHIR_REGISTER_CLASS(Stub, MultiDimArray, stub, multidimarray, stub_multidimarray_method_entry, 0);

	return SUCCESS;
}

/**
 * The exact snippet from issue #1884: nested range loops building an
 * (size+1) x (size+1) matrix of 1s. `i`/`j` are dynamic vars, so the
 * inner write compiles to zephir_array_update_multi(..., SL("zz"), ...).
 */
PHP_METHOD(Stub_MultiDimArray, generate)
{
	zend_bool _0, _4$$3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *size_param = NULL, output, i, j, _3$$3, _7$$4;
	zend_long size, _1, _2, _5$$3, _6$$3;

	ZVAL_UNDEF(&output);
	ZVAL_UNDEF(&i);
	ZVAL_UNDEF(&j);
	ZVAL_UNDEF(&_3$$3);
	ZVAL_UNDEF(&_7$$4);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(size)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &size_param);
	ZEPHIR_INIT_VAR(&output);
	array_init(&output);
	ZEPHIR_INIT_VAR(&i);
	ZVAL_LONG(&i, 0);
	ZEPHIR_INIT_VAR(&j);
	ZVAL_LONG(&j, 0);
	_2 = size;
	_1 = 0;
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
			ZEPHIR_INIT_NVAR(&_3$$3);
			array_init(&_3$$3);
			zephir_array_update_zval(&output, &i, &_3$$3, PH_COPY | PH_SEPARATE);
			_6$$3 = size;
			_5$$3 = 0;
			_4$$3 = 0;
			if (_5$$3 <= _6$$3) {
				while (1) {
					if (_4$$3) {
						_5$$3++;
						if (!(_5$$3 <= _6$$3)) {
							break;
						}
					} else {
						_4$$3 = 1;
					}
					ZEPHIR_INIT_NVAR(&j);
					ZVAL_LONG(&j, _5$$3);
					ZEPHIR_INIT_NVAR(&_7$$4);
					ZVAL_LONG(&_7$$4, 1);
					zephir_array_update_multi(&output, &_7$$4, SL("zz"), 2, &i, &j);
				}
			}
		}
	}
	RETURN_CCTOR(&output);
}

/**
 * Same build with int-typed loop vars: the inner offsets resolve to
 * literal-long keys, i.e. SL("ll") instead of SL("zz").
 */
PHP_METHOD(Stub_MultiDimArray, generateTyped)
{
	zend_bool _0, _4$$3;
	zval output;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *size_param = NULL, _3$$3, _7$$4;
	zend_long size, i, j, _1, _2, _5$$3, _6$$3;

	ZVAL_UNDEF(&_3$$3);
	ZVAL_UNDEF(&_7$$4);
	ZVAL_UNDEF(&output);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(size)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &size_param);
	ZEPHIR_INIT_VAR(&output);
	array_init(&output);
	i = 0;
	j = 0;
	_2 = size;
	_1 = 0;
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
			i = _1;
			ZEPHIR_INIT_NVAR(&_3$$3);
			array_init(&_3$$3);
			zephir_array_update_long(&output, i, &_3$$3, PH_COPY | PH_SEPARATE ZEPHIR_DEBUG_PARAMS_DUMMY);
			_6$$3 = size;
			_5$$3 = 0;
			_4$$3 = 0;
			if (_5$$3 <= _6$$3) {
				while (1) {
					if (_4$$3) {
						_5$$3++;
						if (!(_5$$3 <= _6$$3)) {
							break;
						}
					} else {
						_4$$3 = 1;
					}
					j = _5$$3;
					ZEPHIR_INIT_NVAR(&_7$$4);
					ZVAL_LONG(&_7$$4, 1);
					zephir_array_update_multi(&output, &_7$$4, SL("ll"), 2, i, j);
				}
			}
		}
	}
	RETURN_CTOR(&output);
}

/**
 * Literal long keys at both levels: SL("ll").
 */
PHP_METHOD(Stub_MultiDimArray, intKeys)
{
	zval a, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	array_init(&a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 10);
	zephir_array_update_multi(&a, &_0, SL("ll"), 2, 0, 1);
	RETURN_CCTOR(&a);
}

/**
 * Literal string keys at both levels: SL("ss").
 */
PHP_METHOD(Stub_MultiDimArray, stringKeys)
{
	zval a, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	array_init(&a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 10);
	zephir_array_update_multi(&a, &_0, SL("ss"), 4, SL("x"), SL("y"));
	RETURN_CCTOR(&a);
}

/**
 * Three levels of dynamic string keys: SL("sss").
 */
PHP_METHOD(Stub_MultiDimArray, threeLevel)
{
	zval a, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	array_init(&a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 10);
	zephir_array_update_multi(&a, &_0, SL("sss"), 6, SL("x"), SL("y"), SL("z"));
	RETURN_CCTOR(&a);
}

/**
 * Mixed key kinds down the chain: SL("sl") and SL("ls").
 */
PHP_METHOD(Stub_MultiDimArray, mixedKeys)
{
	zval a, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	array_init(&a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_update_multi(&a, &_0, SL("sl"), 3, SL("k"), 0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 2);
	zephir_array_update_multi(&a, &_0, SL("ls"), 3, 1, SL("k"));
	RETURN_CCTOR(&a);
}

/**
 * Append on the final level: SL("la") / SL("sa").
 */
PHP_METHOD(Stub_MultiDimArray, appendFinal)
{
	zval a, _0, _1, _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	array_init(&a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "first");
	zephir_array_update_multi(&a, &_0, SL("la"), 2, 0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "second");
	zephir_array_update_multi(&a, &_1, SL("la"), 2, 0);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_LONG(&_2, 1);
	zephir_array_update_multi(&a, &_2, SL("sa"), 3, SL("bag"));
	RETURN_CCTOR(&a);
}

/**
 * Overwrite the same final slot twice. Exercises the last-level store
 * path where the pre-fetch previously found an existing scalar.
 */
PHP_METHOD(Stub_MultiDimArray, overwriteFinal)
{
	zval a, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	array_init(&a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_update_multi(&a, &_0, SL("ll"), 2, 0, 0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 2);
	zephir_array_update_multi(&a, &_0, SL("ll"), 2, 0, 0);
	RETURN_CCTOR(&a);
}

/**
 * The final slot first holds an array, then is overwritten by a scalar.
 * Exercises the branch where the pre-fetch previously found an array.
 */
PHP_METHOD(Stub_MultiDimArray, overwriteArrayWithScalar)
{
	zval _0;
	zval a, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	array_init(&a);
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
	zephir_array_update_multi(&a, &_0, SL("ss"), 4, SL("p"), SL("q"));
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 99);
	zephir_array_update_multi(&a, &_1, SL("ss"), 4, SL("p"), SL("q"));
	RETURN_CCTOR(&a);
}

