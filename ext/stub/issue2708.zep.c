
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
#include "kernel/array.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/operators.h"


/**
 * A literal string array offset must resolve to the key PHP resolves it to.
 *
 * PHP folds a constant numeric string subscript to an integer key while it
 * compiles (Zend/zend_compile.c, zend_handle_numeric_dim), so `$a["3"]` is
 * `$a[3]`. Zephir's literal-offset kernel helpers reached `zend_hash_str_*`
 * directly and so kept the string key, while every zval-indexed counterpart
 * went through `zend_symtable_*` and normalised. The two disagreed even within
 * one Zephir program: a constant array literal is emitted as `add_assoc_*_ex`,
 * which php-src implements with `zend_symtable_str_update`.
 *
 * An ArrayAccess container is the exception PHP itself carves out: it receives
 * the original string, never the folded integer.
 *
 * @link https://github.com/zephir-lang/zephir/issues/2708
 */
ZEPHIR_INIT_CLASS(Stub_Issue2708)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2708, stub, issue2708, stub_issue2708_method_entry, 0);

	return SUCCESS;
}

/**
 * The literal stores the integer key, so the literal offset has to find it.
 */
PHP_METHOD(Stub_Issue2708, issetOnConstantLiteral)
{
	zval a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&a);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 1, 0);
	add_assoc_long_ex(&a, SL("3"), 1);
	RETURN_MM_BOOL(zephir_array_isset_value_string(&a, SL("3")));
}

PHP_METHOD(Stub_Issue2708, emptyOnConstantLiteral)
{
	zval a, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 1, 0);
	add_assoc_long_ex(&a, SL("3"), 1);
	zephir_memory_observe(&_0);
	zephir_array_fetch_string(&_0, &a, SL("3"), PH_NOISY, "stub/issue2708.zep", 39);
	RETURN_MM_BOOL(ZEPHIR_IS_EMPTY(&_0));
}

PHP_METHOD(Stub_Issue2708, fetchOnConstantLiteral)
{
	zval a, value;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&value);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 1, 0);
	add_assoc_stringl_ex(&a, SL("3"), SL("found"));
	zephir_memory_observe(&value);
	if (zephir_array_isset_string_fetch(&value, &a, SL("3"), 0)) {
		RETURN_CCTOR(&value);
	}
	RETURN_MM_NULL();
}

PHP_METHOD(Stub_Issue2708, readOnConstantLiteral)
{
	zval a, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 1, 0);
	add_assoc_stringl_ex(&a, SL("3"), SL("found"));
	zephir_array_fetch_string(&_0, &a, SL("3"), PH_NOISY | PH_READONLY, "stub/issue2708.zep", 61);
	RETURN_CTOR(&_0);
}

PHP_METHOD(Stub_Issue2708, unsetLiteralOffset)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL;
	zval a;

	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	zephir_get_arrval(&a, a_param);
	zephir_array_unset_string(&a, SL("3"), PH_SEPARATE);
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2708, writeThenReadBack)
{
	zval a, _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	array_init(&a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "written");
	zephir_array_update_string(&a, SL("3"), &_0, PH_COPY | PH_SEPARATE);
	zephir_array_fetch_long(&_1, &a, 3, PH_NOISY | PH_READONLY, "stub/issue2708.zep", 78);
	RETURN_CTOR(&_1);
}

/**
 * `zephir_array_update_multi`'s string slot delegates to the same helper,
 * so the nested write has to agree with the single one.
 */
PHP_METHOD(Stub_Issue2708, writeMultiDimensional)
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
	ZVAL_STRING(&_0, "deep");
	zephir_array_update_multi(&a, &_0, SL("ss"), 4, SL("3"), SL("4"));
	RETURN_CCTOR(&a);
}

PHP_METHOD(Stub_Issue2708, writeThree)
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
	zephir_array_update_string(&a, SL("3"), &_0, PH_COPY | PH_SEPARATE);
	RETURN_CCTOR(&a);
}

PHP_METHOD(Stub_Issue2708, writeZero)
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
	zephir_array_update_string(&a, SL("0"), &_0, PH_COPY | PH_SEPARATE);
	RETURN_CCTOR(&a);
}

PHP_METHOD(Stub_Issue2708, writeNegativeThree)
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
	zephir_array_update_string(&a, SL("-3"), &_0, PH_COPY | PH_SEPARATE);
	RETURN_CCTOR(&a);
}

PHP_METHOD(Stub_Issue2708, writeIntMax)
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
	zephir_array_update_string(&a, SL("9223372036854775807"), &_0, PH_COPY | PH_SEPARATE);
	RETURN_CCTOR(&a);
}

PHP_METHOD(Stub_Issue2708, writeLeadingZero)
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
	zephir_array_update_string(&a, SL("03"), &_0, PH_COPY | PH_SEPARATE);
	RETURN_CCTOR(&a);
}

PHP_METHOD(Stub_Issue2708, writeLeadingPlus)
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
	zephir_array_update_string(&a, SL("+3"), &_0, PH_COPY | PH_SEPARATE);
	RETURN_CCTOR(&a);
}

PHP_METHOD(Stub_Issue2708, writeNegativeZero)
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
	zephir_array_update_string(&a, SL("-0"), &_0, PH_COPY | PH_SEPARATE);
	RETURN_CCTOR(&a);
}

PHP_METHOD(Stub_Issue2708, writeLeadingSpace)
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
	zephir_array_update_string(&a, SL(" 3"), &_0, PH_COPY | PH_SEPARATE);
	RETURN_CCTOR(&a);
}

PHP_METHOD(Stub_Issue2708, writeTrailingSpace)
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
	zephir_array_update_string(&a, SL("3 "), &_0, PH_COPY | PH_SEPARATE);
	RETURN_CCTOR(&a);
}

PHP_METHOD(Stub_Issue2708, writeDecimalPoint)
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
	zephir_array_update_string(&a, SL("3.0"), &_0, PH_COPY | PH_SEPARATE);
	RETURN_CCTOR(&a);
}

PHP_METHOD(Stub_Issue2708, writeOverflow)
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
	zephir_array_update_string(&a, SL("9223372036854775808"), &_0, PH_COPY | PH_SEPARATE);
	RETURN_CCTOR(&a);
}

/**
 * An integer key moves `nNextFreeElement`, so the append that follows
 * lands past it rather than at zero.
 */
PHP_METHOD(Stub_Issue2708, appendAfterNumericStringKey)
{
	zval a, _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	array_init(&a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "x");
	zephir_array_update_string(&a, SL("3"), &_0, PH_COPY | PH_SEPARATE);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "y");
	zephir_array_append(&a, &_1, PH_SEPARATE, "stub/issue2708.zep", 215);
	RETURN_CCTOR(&a);
}

/**
 * array_merge() renumbers integer keys and preserves string ones, so the
 * key type is observable from the merged result.
 */
PHP_METHOD(Stub_Issue2708, mergeAfterNumericStringKey)
{
	zval _1;
	zval a, _0, _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	array_init(&a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "x");
	zephir_array_update_string(&a, SL("3"), &_0, PH_COPY | PH_SEPARATE);
	ZEPHIR_INIT_VAR(&_1);
	zephir_create_array(&_1, 1, 0);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_STRING(&_2, "y");
	zephir_array_fast_append(&_1, &_2);
	zephir_fast_array_merge(return_value, &a, &_1);
	RETURN_MM();
}

/**
 * The control: an ordinary string key must stay exactly what it is.
 */
PHP_METHOD(Stub_Issue2708, writeOrdinaryKey)
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
	zephir_array_update_string(&a, SL("key_a"), &_0, PH_COPY | PH_SEPARATE);
	RETURN_CCTOR(&a);
}

PHP_METHOD(Stub_Issue2708, readOnContainer)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *container, container_sub, _0;

	ZVAL_UNDEF(&container_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(container)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &container);
	zephir_memory_observe(&_0);
	zephir_array_fetch_string(&_0, container, SL("3"), PH_NOISY, "stub/issue2708.zep", 249);
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_Issue2708, issetOnContainer)
{
	zval *container, container_sub;

	ZVAL_UNDEF(&container_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(container)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &container);
	RETURN_BOOL(zephir_array_isset_value_string(container, SL("3")));
}

PHP_METHOD(Stub_Issue2708, writeOnContainer)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *container, container_sub, _0;

	ZVAL_UNDEF(&container_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(container)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &container);
	ZEPHIR_SEPARATE_PARAM(container);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "v");
	zephir_array_update_string(container, SL("3"), &_0, PH_COPY | PH_SEPARATE);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Issue2708, unsetOnContainer)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *container, container_sub;

	ZVAL_UNDEF(&container_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(container)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &container);
	ZEPHIR_SEPARATE_PARAM(container);
	zephir_array_unset_string(container, SL("3"), PH_SEPARATE);
	ZEPHIR_MM_RESTORE();
}

