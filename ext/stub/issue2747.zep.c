
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
#include "kernel/concat.h"
#include "kernel/fcall.h"
#include "kernel/string.h"


/**
 * @issue https://github.com/zephir-lang/zephir/issues/2747
 *
 * A compound assignment on an array element reads the element, applies the
 * operator and writes the result back, as PHP's ZEND_ASSIGN_DIM_OP does. It
 * must never overwrite the element with the right-hand side. Every container
 * form is here: a local, an object property and a static property, with one
 * offset, several offsets and a trailing append. The test compares each
 * against the same PHP statement.
 */
ZEPHIR_INIT_CLASS(Stub_Issue2747)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2747, stub, issue2747, stub_issue2747_method_entry, 0);

	zend_declare_property_null(stub_issue2747_ce, SL("p"), ZEND_ACC_PUBLIC);
	zend_declare_property_null(stub_issue2747_ce, SL("sp"), ZEND_ACC_PUBLIC|ZEND_ACC_STATIC);
	/**
	 * Evaluation order: PHP evaluates an index expression before the
	 * right-hand side, except a plain variable index, which is read when the
	 * element is written. `trace` records each evaluation.
	 */
	zend_declare_property_null(stub_issue2747_ce, SL("trace"), ZEND_ACC_PUBLIC);
	zend_declare_property_long(stub_issue2747_ce, SL("counter"), 0, ZEND_ACC_PUBLIC);
	stub_issue2747_ce->create_object = zephir_init_properties_Stub_Issue2747;

	return SUCCESS;
}

PHP_METHOD(Stub_Issue2747, add)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, *k, k_sub, *v, v_sub;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&v_sub);
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(k)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 3, 0, &a, &k, &v);
	ZEPHIR_SEPARATE_PARAM(a);
	zephir_array_assign_op(a, v, add_function, SL("z"), 1, k);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETVAL_ZVAL(a, 1, 0);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2747, sub)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, *k, k_sub, *v, v_sub;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&v_sub);
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(k)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 3, 0, &a, &k, &v);
	ZEPHIR_SEPARATE_PARAM(a);
	zephir_array_assign_op(a, v, sub_function, SL("z"), 1, k);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETVAL_ZVAL(a, 1, 0);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2747, mul)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, *k, k_sub, *v, v_sub;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&v_sub);
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(k)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 3, 0, &a, &k, &v);
	ZEPHIR_SEPARATE_PARAM(a);
	zephir_array_assign_op(a, v, mul_function, SL("z"), 1, k);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETVAL_ZVAL(a, 1, 0);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2747, div)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, *k, k_sub, *v, v_sub;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&v_sub);
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(k)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 3, 0, &a, &k, &v);
	ZEPHIR_SEPARATE_PARAM(a);
	zephir_array_assign_op(a, v, div_function, SL("z"), 1, k);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETVAL_ZVAL(a, 1, 0);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2747, mod)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, *k, k_sub, *v, v_sub;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&v_sub);
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(k)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 3, 0, &a, &k, &v);
	ZEPHIR_SEPARATE_PARAM(a);
	zephir_array_assign_op(a, v, mod_function, SL("z"), 1, k);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETVAL_ZVAL(a, 1, 0);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2747, concat)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, *k, k_sub, *v, v_sub;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&v_sub);
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(k)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 3, 0, &a, &k, &v);
	ZEPHIR_SEPARATE_PARAM(a);
	zephir_array_assign_op(a, v, concat_function, SL("z"), 1, k);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETVAL_ZVAL(a, 1, 0);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2747, bitwiseAnd)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, *k, k_sub, *v, v_sub;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&v_sub);
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(k)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 3, 0, &a, &k, &v);
	ZEPHIR_SEPARATE_PARAM(a);
	zephir_array_assign_op(a, v, bitwise_and_function, SL("z"), 1, k);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETVAL_ZVAL(a, 1, 0);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2747, bitwiseOr)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, *k, k_sub, *v, v_sub;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&v_sub);
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(k)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 3, 0, &a, &k, &v);
	ZEPHIR_SEPARATE_PARAM(a);
	zephir_array_assign_op(a, v, bitwise_or_function, SL("z"), 1, k);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETVAL_ZVAL(a, 1, 0);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2747, bitwiseXor)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, *k, k_sub, *v, v_sub;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&v_sub);
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(k)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 3, 0, &a, &k, &v);
	ZEPHIR_SEPARATE_PARAM(a);
	zephir_array_assign_op(a, v, bitwise_xor_function, SL("z"), 1, k);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETVAL_ZVAL(a, 1, 0);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2747, shiftLeft)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, *k, k_sub, *v, v_sub;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&v_sub);
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(k)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 3, 0, &a, &k, &v);
	ZEPHIR_SEPARATE_PARAM(a);
	zephir_array_assign_op(a, v, shift_left_function, SL("z"), 1, k);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETVAL_ZVAL(a, 1, 0);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2747, shiftRight)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, *k, k_sub, *v, v_sub;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&v_sub);
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(k)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 3, 0, &a, &k, &v);
	ZEPHIR_SEPARATE_PARAM(a);
	zephir_array_assign_op(a, v, shift_right_function, SL("z"), 1, k);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETVAL_ZVAL(a, 1, 0);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2747, addStringLiteralKey)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, *v, v_sub;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&v_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &a_param, &v);
	zephir_get_arrval(&a, a_param);
	zephir_array_assign_op(&a, v, add_function, SL("s"), 2, SL("k"));
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2747, addIntLiteralKey)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, *v, v_sub;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&v_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &a_param, &v);
	zephir_get_arrval(&a, a_param);
	zephir_array_assign_op(&a, v, add_function, SL("l"), 1, (zend_long) 1);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2747, addLongKey)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long k;
	zval *a_param = NULL, *k_param = NULL, *v, v_sub;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&v_sub);
	ZEND_PARSE_PARAMETERS_START(3, 3)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
		Z_PARAM_LONG(k)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 3, 0, &a_param, &k_param, &v);
	zephir_get_arrval(&a, a_param);
	zephir_array_assign_op(&a, v, add_function, SL("l"), 1, (zend_long) k);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2747, addStringKey)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_string *k = NULL;
	zval *a_param = NULL, k_zv, *v, v_sub;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&k_zv);
	ZVAL_UNDEF(&v_sub);
	ZEND_PARSE_PARAMETERS_START(3, 3)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
		Z_PARAM_STR(k)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	a_param = ZEND_CALL_ARG(execute_data, 1);
	v = ZEND_CALL_ARG(execute_data, 3);
	zephir_get_arrval(&a, a_param);
	zephir_memory_observe(&k_zv);
	ZVAL_STR_COPY(&k_zv, k);
	zephir_array_assign_op(&a, v, add_function, SL("z"), 1, &k_zv);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2747, addIntLiteral)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, _0;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 2);
	zephir_array_assign_op(&a, &_0, add_function, SL("s"), 2, SL("k"));
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2747, concatStringLiteral)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, _0;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "x");
	zephir_array_assign_op(&a, &_0, concat_function, SL("s"), 2, SL("k"));
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2747, addLongValue)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long v;
	zval *a, a_sub, *k, k_sub, *v_param = NULL, _0;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(k)
		Z_PARAM_LONG(v)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 3, 0, &a, &k, &v_param);
	ZEPHIR_SEPARATE_PARAM(a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, v);
	zephir_array_assign_op(a, &_0, add_function, SL("z"), 1, k);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETVAL_ZVAL(a, 1, 0);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2747, mulDoubleValue)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	double v;
	zval *a, a_sub, *k, k_sub, *v_param = NULL, _0;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(k)
		Z_PARAM_DOUBLE(v)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 3, 0, &a, &k, &v_param);
	ZEPHIR_SEPARATE_PARAM(a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_DOUBLE(&_0, v);
	zephir_array_assign_op(a, &_0, mul_function, SL("z"), 1, k);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETVAL_ZVAL(a, 1, 0);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2747, concatOwnElement)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, _0;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	zephir_get_arrval(&a, a_param);
	zephir_array_fetch_string(&_0, &a, SL("k"), PH_NOISY | PH_READONLY, "stub/issue2747.zep", 154);
	zephir_array_assign_op(&a, &_0, concat_function, SL("s"), 2, SL("k"));
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CTOR(&a);
}

PHP_METHOD(Stub_Issue2747, addToCopy)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, *k, k_sub, *v, v_sub, b;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&v_sub);
	ZVAL_UNDEF(&b);
	ZEND_PARSE_PARAMETERS_START(3, 3)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
		Z_PARAM_ZVAL(k)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 3, 0, &a_param, &k, &v);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_CPY_WRT(&b, &a);
	zephir_array_assign_op(&b, v, add_function, SL("z"), 1, k);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_create_array(return_value, 2, 0);
	zephir_array_fast_append(return_value, &a);
	zephir_array_fast_append(return_value, &b);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2747, addNested)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, *i, i_sub, *j, j_sub, *v, v_sub;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&i_sub);
	ZVAL_UNDEF(&j_sub);
	ZVAL_UNDEF(&v_sub);
	ZEND_PARSE_PARAMETERS_START(4, 4)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(i)
		Z_PARAM_ZVAL(j)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 4, 0, &a, &i, &j, &v);
	ZEPHIR_SEPARATE_PARAM(a);
	zephir_array_assign_op(a, v, add_function, SL("zz"), 2, i, j);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETVAL_ZVAL(a, 1, 0);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2747, concatNestedLiteral)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, *v, v_sub;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&v_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &a, &v);
	ZEPHIR_SEPARATE_PARAM(a);
	zephir_array_assign_op(a, v, concat_function, SL("ls"), 3, (zend_long) 1, SL("x"));
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETVAL_ZVAL(a, 1, 0);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2747, subAppend)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, *v, v_sub;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&v_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &a, &v);
	ZEPHIR_SEPARATE_PARAM(a);
	zephir_array_assign_op(a, v, sub_function, SL("a"), 1);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETVAL_ZVAL(a, 1, 0);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2747, concatNestedAppend)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, *k, k_sub, *v, v_sub;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&v_sub);
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(k)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 3, 0, &a, &k, &v);
	ZEPHIR_SEPARATE_PARAM(a);
	zephir_array_assign_op(a, v, concat_function, SL("za"), 2, k);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETVAL_ZVAL(a, 1, 0);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2747, addThis)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *k, k_sub, *v, v_sub, _0, *_1;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&v_sub);
	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("p", 1, 1);
	}

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(k)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k, &v);
	zephir_memory_observe(&_0);
	_1 = zephir_fetch_property_rw(this_ptr, _zephir_prop_0, &_0);
	zephir_array_assign_op(_1, v, add_function, SL("z"), 1, k);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_MEMBER(getThis(), "p");
}

PHP_METHOD(Stub_Issue2747, addThisNested)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *k, k_sub, *j, j_sub, *v, v_sub, _0, *_1;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&j_sub);
	ZVAL_UNDEF(&v_sub);
	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("p", 1, 1);
	}

	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_ZVAL(k)
		Z_PARAM_ZVAL(j)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 3, 0, &k, &j, &v);
	zephir_memory_observe(&_0);
	_1 = zephir_fetch_property_rw(this_ptr, _zephir_prop_0, &_0);
	zephir_array_assign_op(_1, v, add_function, SL("zz"), 2, k, j);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_MEMBER(getThis(), "p");
}

PHP_METHOD(Stub_Issue2747, subThisAppend)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *v, v_sub, _0, *_1;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&v_sub);
	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("p", 1, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &v);
	zephir_memory_observe(&_0);
	_1 = zephir_fetch_property_rw(this_ptr, _zephir_prop_0, &_0);
	zephir_array_assign_op(_1, v, sub_function, SL("a"), 1);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_MEMBER(getThis(), "p");
}

PHP_METHOD(Stub_Issue2747, concatThisNestedAppend)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *k, k_sub, *v, v_sub, _0, *_1;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&v_sub);
	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("p", 1, 1);
	}

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(k)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k, &v);
	zephir_memory_observe(&_0);
	_1 = zephir_fetch_property_rw(this_ptr, _zephir_prop_0, &_0);
	zephir_array_assign_op(_1, v, concat_function, SL("za"), 2, k);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_MEMBER(getThis(), "p");
}

PHP_METHOD(Stub_Issue2747, addObject)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *o, o_sub, *k, k_sub, *v, v_sub, _0, *_1;

	ZVAL_UNDEF(&o_sub);
	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&v_sub);
	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("p", 1, 1);
	}

	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_ZVAL(o)
		Z_PARAM_ZVAL(k)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 3, 0, &o, &k, &v);
	ZEPHIR_SEPARATE_PARAM(o);
	zephir_memory_observe(&_0);
	_1 = zephir_fetch_property_rw(o, _zephir_prop_0, &_0);
	zephir_array_assign_op(_1, v, add_function, SL("z"), 1, k);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETVAL_ZVAL(o, 1, 0);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2747, addStatic)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *k, k_sub, *v, v_sub, _0, *_1, _2;

	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&v_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(k)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k, &v);
	zephir_memory_observe(&_0);
	_1 = zephir_fetch_static_property_rw_ce(stub_issue2747_ce, SL("sp"), &_0);
	zephir_array_assign_op(_1, v, add_function, SL("z"), 1, k);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_read_static_property_ce(&_2, stub_issue2747_ce, SL("sp"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTOR(&_2);
}

PHP_METHOD(Stub_Issue2747, addStaticNested)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *k, k_sub, *j, j_sub, *v, v_sub, _0, *_1, _2;

	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&j_sub);
	ZVAL_UNDEF(&v_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_ZVAL(k)
		Z_PARAM_ZVAL(j)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 3, 0, &k, &j, &v);
	zephir_memory_observe(&_0);
	_1 = zephir_fetch_static_property_rw_ce(stub_issue2747_ce, SL("sp"), &_0);
	zephir_array_assign_op(_1, v, add_function, SL("zz"), 2, k, j);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_read_static_property_ce(&_2, stub_issue2747_ce, SL("sp"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTOR(&_2);
}

PHP_METHOD(Stub_Issue2747, subStaticAppend)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *v, v_sub, _0, *_1, _2;

	ZVAL_UNDEF(&v_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &v);
	zephir_memory_observe(&_0);
	_1 = zephir_fetch_static_property_rw_ce(stub_issue2747_ce, SL("sp"), &_0);
	zephir_array_assign_op(_1, v, sub_function, SL("a"), 1);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_read_static_property_ce(&_2, stub_issue2747_ce, SL("sp"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTOR(&_2);
}

PHP_METHOD(Stub_Issue2747, concatStaticNestedAppend)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *k, k_sub, *v, v_sub, _0, *_1, _2;

	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&v_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(k)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &k, &v);
	zephir_memory_observe(&_0);
	_1 = zephir_fetch_static_property_rw_ce(stub_issue2747_ce, SL("sp"), &_0);
	zephir_array_assign_op(_1, v, concat_function, SL("za"), 2, k);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_read_static_property_ce(&_2, stub_issue2747_ce, SL("sp"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTOR(&_2);
}

PHP_METHOD(Stub_Issue2747, setProperties)
{
	zval *value, value_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&value_sub);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("p", 1, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &value);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 76, value);
	zephir_update_static_property_ce(stub_issue2747_ce, ZEND_STRL("sp"), value);
}

PHP_METHOD(Stub_Issue2747, key)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *n, n_sub, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&n_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &n);
	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_CONCAT_SV(&_0, "k", n);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_update_property_array_append(this_ptr, SL("trace"), &_0);
	RETVAL_ZVAL(n, 1, 0);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2747, value)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *n, n_sub, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&n_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &n);
	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_CONCAT_SV(&_0, "v", n);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_update_property_array_append(this_ptr, SL("trace"), &_0);
	RETVAL_ZVAL(n, 1, 0);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2747, bump)
{
	zval *this_ptr = getThis();
	RETURN_ON_FAILURE(zephir_property_incr(this_ptr, SL("counter")));
	RETURN_MEMBER(getThis(), "counter");
}

PHP_METHOD(Stub_Issue2747, orderLocal)
{
	zval a, _0, _1, _2, _3, _4;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_4);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("trace", 5, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	array_init(&a);
	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 77, &_0);
	ZVAL_LONG(&_2, 1);
	ZEPHIR_CALL_METHOD(&_1, this_ptr, "key", NULL, 0, &_2);
	zephir_check_call_status();
	ZVAL_LONG(&_2, 2);
	ZEPHIR_CALL_METHOD(&_3, this_ptr, "value", NULL, 0, &_2);
	zephir_check_call_status();
	zephir_array_update_zval(&a, &_1, &_3, PH_COPY | PH_SEPARATE);
	zephir_create_array(return_value, 2, 0);
	zephir_memory_observe(&_4);
	zephir_read_property_cached(&_4, this_ptr, _zephir_prop_0, 77, PH_NOISY_CC);
	zephir_array_fast_append(return_value, &_4);
	zephir_array_fast_append(return_value, &a);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2747, orderLocalNested)
{
	zval a, _0, _1, _2, _3, _4, _5;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_5);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("trace", 5, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	array_init(&a);
	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 77, &_0);
	ZVAL_LONG(&_2, 1);
	ZEPHIR_CALL_METHOD(&_1, this_ptr, "key", NULL, 0, &_2);
	zephir_check_call_status();
	ZVAL_LONG(&_2, 2);
	ZEPHIR_CALL_METHOD(&_3, this_ptr, "key", NULL, 0, &_2);
	zephir_check_call_status();
	ZVAL_LONG(&_2, 3);
	ZEPHIR_CALL_METHOD(&_4, this_ptr, "value", NULL, 0, &_2);
	zephir_check_call_status();
	zephir_array_update_multi(&a, &_4, SL("zz"), 2, &_1, &_3);
	zephir_create_array(return_value, 2, 0);
	zephir_memory_observe(&_5);
	zephir_read_property_cached(&_5, this_ptr, _zephir_prop_0, 77, PH_NOISY_CC);
	zephir_array_fast_append(return_value, &_5);
	zephir_array_fast_append(return_value, &a);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2747, orderLocalAppend)
{
	zval a, _0, _1, _2, _3, _4;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_4);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("trace", 5, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	array_init(&a);
	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 77, &_0);
	ZVAL_LONG(&_2, 1);
	ZEPHIR_CALL_METHOD(&_1, this_ptr, "key", NULL, 0, &_2);
	zephir_check_call_status();
	ZVAL_LONG(&_2, 2);
	ZEPHIR_CALL_METHOD(&_3, this_ptr, "value", NULL, 0, &_2);
	zephir_check_call_status();
	zephir_array_update_multi(&a, &_3, SL("za"), 2, &_1);
	zephir_create_array(return_value, 2, 0);
	zephir_memory_observe(&_4);
	zephir_read_property_cached(&_4, this_ptr, _zephir_prop_0, 77, PH_NOISY_CC);
	zephir_array_fast_append(return_value, &_4);
	zephir_array_fast_append(return_value, &a);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2747, orderCompound)
{
	zval a, _0, _1, _2, _3, _4;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_4);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("trace", 5, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 1, 0);
	add_index_stringl(&a, 1, SL("a"));
	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 77, &_0);
	ZVAL_LONG(&_2, 1);
	ZEPHIR_CALL_METHOD(&_1, this_ptr, "key", NULL, 0, &_2);
	zephir_check_call_status();
	ZVAL_LONG(&_2, 2);
	ZEPHIR_CALL_METHOD(&_3, this_ptr, "value", NULL, 0, &_2);
	zephir_check_call_status();
	zephir_array_assign_op(&a, &_3, concat_function, SL("z"), 1, &_1);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_create_array(return_value, 2, 0);
	zephir_memory_observe(&_4);
	zephir_read_property_cached(&_4, this_ptr, _zephir_prop_0, 77, PH_NOISY_CC);
	zephir_array_fast_append(return_value, &_4);
	zephir_array_fast_append(return_value, &a);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2747, orderThis)
{
	zval _0, _1, _2, _3, _4, _5;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_5);
	static zend_string *_zephir_prop_0 = NULL;
	static zend_string *_zephir_prop_1 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("trace", 5, 1);
	}
	if (UNEXPECTED(!_zephir_prop_1)) {
		_zephir_prop_1 = zend_string_init("p", 1, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 77, &_0);
	ZEPHIR_INIT_VAR(&_1);
	array_init(&_1);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_1, 76, &_1);
	ZVAL_LONG(&_3, 1);
	ZEPHIR_CALL_METHOD(&_2, this_ptr, "key", NULL, 0, &_3);
	zephir_check_call_status();
	ZVAL_LONG(&_3, 2);
	ZEPHIR_CALL_METHOD(&_4, this_ptr, "value", NULL, 0, &_3);
	zephir_check_call_status();
	zephir_update_property_array(this_ptr, SL("p"), &_2, &_4);
	zephir_create_array(return_value, 2, 0);
	zephir_memory_observe(&_5);
	zephir_read_property_cached(&_5, this_ptr, _zephir_prop_0, 77, PH_NOISY_CC);
	zephir_array_fast_append(return_value, &_5);
	ZEPHIR_OBS_NVAR(&_5);
	zephir_read_property_cached(&_5, this_ptr, _zephir_prop_1, 76, PH_NOISY_CC);
	zephir_array_fast_append(return_value, &_5);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2747, orderThisNested)
{
	zval _0, _1, _2, _3, _4, _5, _6;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_5);
	ZVAL_UNDEF(&_6);
	static zend_string *_zephir_prop_0 = NULL;
	static zend_string *_zephir_prop_1 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("trace", 5, 1);
	}
	if (UNEXPECTED(!_zephir_prop_1)) {
		_zephir_prop_1 = zend_string_init("p", 1, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 77, &_0);
	ZEPHIR_INIT_VAR(&_1);
	array_init(&_1);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_1, 76, &_1);
	ZVAL_LONG(&_3, 1);
	ZEPHIR_CALL_METHOD(&_2, this_ptr, "key", NULL, 0, &_3);
	zephir_check_call_status();
	ZVAL_LONG(&_3, 2);
	ZEPHIR_CALL_METHOD(&_4, this_ptr, "key", NULL, 0, &_3);
	zephir_check_call_status();
	ZVAL_LONG(&_3, 3);
	ZEPHIR_CALL_METHOD(&_5, this_ptr, "value", NULL, 0, &_3);
	zephir_check_call_status();
	zephir_update_property_array_multi(this_ptr, SL("p"), &_5, SL("zza"), 3, &_2, &_4);
	zephir_create_array(return_value, 2, 0);
	zephir_memory_observe(&_6);
	zephir_read_property_cached(&_6, this_ptr, _zephir_prop_0, 77, PH_NOISY_CC);
	zephir_array_fast_append(return_value, &_6);
	ZEPHIR_OBS_NVAR(&_6);
	zephir_read_property_cached(&_6, this_ptr, _zephir_prop_1, 76, PH_NOISY_CC);
	zephir_array_fast_append(return_value, &_6);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2747, orderStatic)
{
	zval _0, _1, _2, _3, _4, _5, _6;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_5);
	ZVAL_UNDEF(&_6);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("trace", 5, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 77, &_0);
	ZEPHIR_INIT_VAR(&_1);
	array_init(&_1);
	zephir_update_static_property_ce(stub_issue2747_ce, ZEND_STRL("sp"), &_1);
	ZVAL_LONG(&_3, 1);
	ZEPHIR_CALL_METHOD(&_2, this_ptr, "key", NULL, 0, &_3);
	zephir_check_call_status();
	ZVAL_LONG(&_3, 2);
	ZEPHIR_CALL_METHOD(&_4, this_ptr, "key", NULL, 0, &_3);
	zephir_check_call_status();
	ZVAL_LONG(&_3, 3);
	ZEPHIR_CALL_METHOD(&_5, this_ptr, "value", NULL, 0, &_3);
	zephir_check_call_status();
	zephir_update_static_property_array_multi_ce(stub_issue2747_ce, SL("sp"), &_5, SL("zz"), 2, &_2, &_4);
	zephir_create_array(return_value, 2, 0);
	zephir_memory_observe(&_6);
	zephir_read_property_cached(&_6, this_ptr, _zephir_prop_0, 77, PH_NOISY_CC);
	zephir_array_fast_append(return_value, &_6);
	zephir_memory_observe(&_3);
	zephir_read_static_property_ce(&_3, stub_issue2747_ce, SL("sp"), PH_NOISY_CC);
	zephir_array_fast_append(return_value, &_3);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2747, orderStaticAppend)
{
	zval _0, _1, _2, _3, _4, _5;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_5);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("trace", 5, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 77, &_0);
	ZEPHIR_INIT_VAR(&_1);
	array_init(&_1);
	zephir_update_static_property_ce(stub_issue2747_ce, ZEND_STRL("sp"), &_1);
	ZVAL_LONG(&_3, 1);
	ZEPHIR_CALL_METHOD(&_2, this_ptr, "key", NULL, 0, &_3);
	zephir_check_call_status();
	ZVAL_LONG(&_3, 2);
	ZEPHIR_CALL_METHOD(&_4, this_ptr, "value", NULL, 0, &_3);
	zephir_check_call_status();
	zephir_update_static_property_array_multi_ce(stub_issue2747_ce, SL("sp"), &_4, SL("za"), 2, &_2);
	zephir_create_array(return_value, 2, 0);
	zephir_memory_observe(&_5);
	zephir_read_property_cached(&_5, this_ptr, _zephir_prop_0, 77, PH_NOISY_CC);
	zephir_array_fast_append(return_value, &_5);
	zephir_memory_observe(&_3);
	zephir_read_static_property_ce(&_3, stub_issue2747_ce, SL("sp"), PH_NOISY_CC);
	zephir_array_fast_append(return_value, &_3);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2747, orderPropertyIndex)
{
	zval a, _0, _1, _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("counter", 7, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	array_init(&a);
	ZVAL_UNDEF(&_0);
	ZVAL_LONG(&_0, 0);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 78, &_0);
	zephir_memory_observe(&_1);
	zephir_read_property_cached(&_1, this_ptr, _zephir_prop_0, 78, PH_NOISY_CC);
	ZEPHIR_CALL_METHOD(&_2, this_ptr, "bump", NULL, 0);
	zephir_check_call_status();
	zephir_array_update_zval(&a, &_1, &_2, PH_COPY | PH_SEPARATE);
	RETURN_CCTOR(&a);
}

PHP_METHOD(Stub_Issue2747, orderPropertyValue)
{
	zval a, _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("counter", 7, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	array_init(&a);
	ZVAL_UNDEF(&_0);
	ZVAL_LONG(&_0, 0);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 78, &_0);
	ZEPHIR_CALL_METHOD(&_1, this_ptr, "bump", NULL, 0);
	zephir_check_call_status();
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 78, PH_NOISY_CC | PH_READONLY);
	zephir_array_update_zval(&a, &_1, &_0, PH_COPY | PH_SEPARATE);
	RETURN_CCTOR(&a);
}

PHP_METHOD(Stub_Issue2747, orderComputedIndex)
{
	zval a, _0, _1, _2, _3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("counter", 7, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	array_init(&a);
	ZVAL_UNDEF(&_0);
	ZVAL_LONG(&_0, 0);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 78, &_0);
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 78, PH_NOISY_CC | PH_READONLY);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 10);
	ZEPHIR_INIT_VAR(&_2);
	zephir_add_function(&_2, &_0, &_1);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_CALL_METHOD(&_3, this_ptr, "bump", NULL, 0);
	zephir_check_call_status();
	zephir_array_update_zval(&a, &_2, &_3, PH_COPY | PH_SEPARATE);
	RETURN_CCTOR(&a);
}

PHP_METHOD(Stub_Issue2747, orderStringOffset)
{
	zval _0, _1, _2;
	zval s, _3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&s);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("counter", 7, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&s);
	ZVAL_STRING(&s, "abc");
	ZVAL_UNDEF(&_0);
	ZVAL_LONG(&_0, 0);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 78, &_0);
	zephir_memory_observe(&_1);
	zephir_read_property_cached(&_1, this_ptr, _zephir_prop_0, 78, PH_NOISY_CC);
	ZEPHIR_CALL_METHOD(&_2, this_ptr, "bump", NULL, 0);
	zephir_check_call_status();
	zephir_cast_to_string(&_3, &_2);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_string_offset_write_zval(&s, &_1, &_3);
	RETURN_CTOR(&s);
}

PHP_METHOD(Stub_Issue2747, orderVariableIndex)
{
	zval i, f, b, _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&i);
	ZVAL_UNDEF(&f);
	ZVAL_UNDEF(&b);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&i);
	zephir_make_local_reference(&i);

	ZEPHIR_INIT_VAR(&b);
	array_init(&b);
	ZEPHIR_INIT_NVAR(Z_REFVAL_P(&i));
	ZVAL_LONG(Z_REFVAL_P(&i), 0);
	ZEPHIR_INIT_VAR(&_0);
	object_init_ex(&_0, stub_70__closure_ce);
	zephir_update_property_reference(&_0, SL("i"), &i);
	ZEPHIR_INIT_VAR(&f);
	zephir_create_closure_bound(&f, &_0, NULL, stub_70__closure_ce, SL("__invoke"));
	ZEPHIR_CALL_ZVAL_FUNCTION(&_1, &f, NULL, 0);
	zephir_check_call_status();
	zephir_array_update_zval(&b, Z_REFVAL_P(&i), &_1, PH_COPY | PH_SEPARATE);
	RETURN_CCTOR(&b);
}

PHP_METHOD(Stub_Issue2747, orderVariableExpressionIndex)
{
	zval i, f, b, _0, _1, _2, _3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&i);
	ZVAL_UNDEF(&f);
	ZVAL_UNDEF(&b);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&i);
	zephir_make_local_reference(&i);

	ZEPHIR_INIT_VAR(&b);
	array_init(&b);
	ZEPHIR_INIT_NVAR(Z_REFVAL_P(&i));
	ZVAL_LONG(Z_REFVAL_P(&i), 0);
	ZEPHIR_INIT_VAR(&_0);
	object_init_ex(&_0, stub_71__closure_ce);
	zephir_update_property_reference(&_0, SL("i"), &i);
	ZEPHIR_INIT_VAR(&f);
	zephir_create_closure_bound(&f, &_0, NULL, stub_71__closure_ce, SL("__invoke"));
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	ZEPHIR_INIT_VAR(&_2);
	zephir_add_function(&_2, Z_REFVAL_P(&i), &_1);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_CALL_ZVAL_FUNCTION(&_3, &f, NULL, 0);
	zephir_check_call_status();
	zephir_array_update_zval(&b, &_2, &_3, PH_COPY | PH_SEPARATE);
	RETURN_CCTOR(&b);
}

PHP_METHOD(Stub_Issue2747, orderLiteralIndex)
{
	zval a, _0, _1, _2, _4, _5;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zephir_fcall_cache_entry *_3 = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_5);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("trace", 5, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	array_init(&a);
	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 77, &_0);
	ZVAL_LONG(&_2, 1);
	ZEPHIR_CALL_METHOD(&_1, this_ptr, "value", &_3, 0, &_2);
	zephir_check_call_status();
	zephir_array_update_long(&a, 0, &_1, PH_COPY | PH_SEPARATE ZEPHIR_DEBUG_PARAMS_DUMMY);
	ZVAL_LONG(&_2, 2);
	ZEPHIR_CALL_METHOD(&_4, this_ptr, "value", &_3, 0, &_2);
	zephir_check_call_status();
	zephir_array_update_string(&a, SL("k"), &_4, PH_COPY | PH_SEPARATE);
	zephir_create_array(return_value, 2, 0);
	zephir_memory_observe(&_5);
	zephir_read_property_cached(&_5, this_ptr, _zephir_prop_0, 77, PH_NOISY_CC);
	zephir_array_fast_append(return_value, &_5);
	zephir_array_fast_append(return_value, &a);
	RETURN_MM();
}

zend_object *zephir_init_properties_Stub_Issue2747(zend_class_entry *class_type)
{
		zval _3$$4;
	zval _0, _2, _1$$3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
		ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_1$$3);
	ZVAL_UNDEF(&_3$$4);
	

		ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
		zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	
	{
		zval local_this_ptr, *this_ptr = &local_this_ptr;
		ZEPHIR_CREATE_OBJECT(this_ptr, class_type);
		zephir_read_property_ex(&_0, this_ptr, ZEND_STRL("trace"), PH_NOISY_CC | PH_READONLY);
		if (Z_TYPE_P(&_0) == IS_NULL) {
			ZEPHIR_INIT_VAR(&_1$$3);
			array_init(&_1$$3);
			zephir_update_property_zval_ex(this_ptr, ZEND_STRL("trace"), &_1$$3);
		}
		zephir_read_property_ex(&_2, this_ptr, ZEND_STRL("p"), PH_NOISY_CC | PH_READONLY);
		if (Z_TYPE_P(&_2) == IS_NULL) {
			ZEPHIR_INIT_VAR(&_3$$4);
			zephir_create_array(&_3$$4, 1, 0);
			add_assoc_long_ex(&_3$$4, SL("k"), 10);
			zephir_update_property_zval_ex(this_ptr, ZEND_STRL("p"), &_3$$4);
		}
		ZEPHIR_MM_RESTORE();
		return Z_OBJ_P(this_ptr);
	}
}

void zephir_init_static_properties_Stub_Issue2747()
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
		ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 1, 0);
	add_assoc_long_ex(&_0, SL("k"), 10);
	zephir_update_static_property_ce(stub_issue2747_ce, ZEND_STRL("sp"), &_0);
	ZEPHIR_MM_RESTORE();
}

