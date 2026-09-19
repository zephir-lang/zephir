
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
 * Drives <Ns>\Buffer from Zephir source, so the kernel fast paths in
 * kernel/array.c are exercised by compiled code rather than from PHP.
 *
 * The buffer parameters are untyped `var` on purpose: Buffer is a hand-written
 * kernel class, so the compiler has no definition to check a `<Buffer>` hint
 * against.
 */
ZEPHIR_INIT_CLASS(Stub_BufferOps)
{
	ZEPHIR_REGISTER_CLASS(Stub, BufferOps, stub, bufferops, stub_bufferops_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_BufferOps, readAt)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long index;
	zval *buf, buf_sub, *index_param = NULL, _0;

	ZVAL_UNDEF(&buf_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(buf)
		Z_PARAM_LONG(index)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &buf, &index_param);
	zephir_memory_observe(&_0);
	zephir_array_fetch_long(&_0, buf, index, PH_NOISY, "stub/bufferops.zep", 15);
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_BufferOps, writeAt)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long index;
	zval *buf, buf_sub, *index_param = NULL, *value, value_sub;

	ZVAL_UNDEF(&buf_sub);
	ZVAL_UNDEF(&value_sub);
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_ZVAL(buf)
		Z_PARAM_LONG(index)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 3, 0, &buf, &index_param, &value);
	ZEPHIR_SEPARATE_PARAM(buf);
	zephir_array_update_long(buf, index, value, PH_COPY | PH_SEPARATE ZEPHIR_DEBUG_PARAMS_DUMMY);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_BufferOps, has)
{
	zend_long index;
	zval *buf, buf_sub, *index_param = NULL;

	ZVAL_UNDEF(&buf_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(buf)
		Z_PARAM_LONG(index)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &buf, &index_param);
	RETURN_BOOL(zephir_array_isset_value_long(buf, index));
}

PHP_METHOD(Stub_BufferOps, size)
{
	zval *buf, buf_sub;

	ZVAL_UNDEF(&buf_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(buf)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &buf);
	RETURN_LONG(zephir_fast_count_int(buf));
}

PHP_METHOD(Stub_BufferOps, sum)
{
	zend_long i, n;
	double total;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *buf, buf_sub, _0$$3;

	ZVAL_UNDEF(&buf_sub);
	ZVAL_UNDEF(&_0$$3);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(buf)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &buf);
	total =  (0.0);
	i = 0;
	n = 0;
	n = zephir_fast_count_int(buf);
	while (1) {
		if (!(i < n)) {
			break;
		}
		ZEPHIR_OBS_NVAR(&_0$$3);
		zephir_array_fetch_long(&_0$$3, buf, i, PH_NOISY, "stub/bufferops.zep", 41);
		total +=  (zephir_get_doubleval(&_0$$3));
		i++;
	}
	RETURN_MM_DOUBLE(total);
}

PHP_METHOD(Stub_BufferOps, scale)
{
	zend_long i, n;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	double factor;
	zval *buf, buf_sub, *factor_param = NULL, _0$$3, _1$$3;

	ZVAL_UNDEF(&buf_sub);
	ZVAL_UNDEF(&_0$$3);
	ZVAL_UNDEF(&_1$$3);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(buf)
		Z_PARAM_ZVAL(factor_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &buf, &factor_param);
	ZEPHIR_SEPARATE_PARAM(buf);
	factor = zephir_get_doubleval(factor_param);
	i = 0;
	n = 0;
	n = zephir_fast_count_int(buf);
	while (1) {
		if (!(i < n)) {
			break;
		}
		ZEPHIR_OBS_NVAR(&_0$$3);
		zephir_array_fetch_long(&_0$$3, buf, i, PH_NOISY, "stub/bufferops.zep", 55);
		ZEPHIR_INIT_NVAR(&_1$$3);
		ZVAL_DOUBLE(&_1$$3, (double) (zephir_get_doubleval(&_0$$3) * factor));
		zephir_array_update_long(buf, i, &_1$$3, PH_COPY | PH_SEPARATE ZEPHIR_DEBUG_PARAMS_DUMMY);
		i++;
	}
	ZEPHIR_MM_RESTORE();
}

