
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
#include "kernel/concat.h"
#include "kernel/fcall.h"
#include "kernel/string.h"


/**
 * Declaring a local with an expression default value must compile.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2394
 */
ZEPHIR_INIT_CLASS(Stub_Issue2394)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2394, stub, issue2394, stub_issue2394_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Issue2394, helper)
{

	RETURN_LONG(7);
}

PHP_METHOD(Stub_Issue2394, addition)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, x;
	zend_long a;

	ZVAL_UNDEF(&x);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(a)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	ZEPHIR_INIT_VAR(&x);
	ZVAL_LONG(&x, (a + 1));
	RETURN_CCTOR(&x);
}

PHP_METHOD(Stub_Issue2394, subtraction)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, x;
	zend_long a;

	ZVAL_UNDEF(&x);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(a)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	ZEPHIR_INIT_VAR(&x);
	ZVAL_LONG(&x, (a - 1));
	RETURN_CCTOR(&x);
}

PHP_METHOD(Stub_Issue2394, multiplication)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, x;
	zend_long a;

	ZVAL_UNDEF(&x);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(a)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	ZEPHIR_INIT_VAR(&x);
	ZVAL_LONG(&x, (a * 2));
	RETURN_CCTOR(&x);
}

PHP_METHOD(Stub_Issue2394, division)
{
	double x;
	zval *a_param = NULL;
	zend_long a;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &a_param);
	x =  (zephir_safe_div_long_long(a, 2));
	RETURN_DOUBLE(x);
}

PHP_METHOD(Stub_Issue2394, modulo)
{
	zval *a_param = NULL;
	zend_long a, x;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &a_param);
	x = zephir_safe_mod_long_long(a, 3);
	RETURN_LONG(x);
}

PHP_METHOD(Stub_Issue2394, concatenation)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a_zv, b_zv, s;
	zend_string *a = NULL, *b = NULL;

	ZVAL_UNDEF(&a_zv);
	ZVAL_UNDEF(&b_zv);
	ZVAL_UNDEF(&s);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(a)
		Z_PARAM_STR(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&a_zv);
	ZVAL_STR_COPY(&a_zv, a);
	zephir_memory_observe(&b_zv);
	ZVAL_STR_COPY(&b_zv, b);
	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_CONCAT_VV(&_0, &a_zv, &b_zv);
	ZEPHIR_CPY_WRT(&s, &_0);
	RETURN_CCTOR(&s);
}

PHP_METHOD(Stub_Issue2394, ternary)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *n_param = NULL, t;
	zend_long n;

	ZVAL_UNDEF(&t);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &n_param);
	if (n < 2) {
		ZEPHIR_INIT_VAR(&t);
		ZVAL_LONG(&t, 10);
	} else {
		ZEPHIR_INIT_NVAR(&t);
		ZVAL_LONG(&t, 20);
	}
	RETURN_CCTOR(&t);
}

PHP_METHOD(Stub_Issue2394, comparison)
{
	zend_bool b;
	zval *n_param = NULL;
	zend_long n;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &n_param);
	b = n > 2;
	RETURN_BOOL(b);
}

PHP_METHOD(Stub_Issue2394, methodCall)
{
	zval r;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&r);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_CALL_METHOD(&r, this_ptr, "helper", NULL, 0);
	zephir_check_call_status();
	RETURN_CCTOR(&r);
}

/**
 * Reproduces the exact shape from the issue: an expression-default
 * declaration nested inside a loop with a preceding `continue`.
 */
PHP_METHOD(Stub_Issue2394, backslashLoop)
{
	zend_long i = 0, kos$$4;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval format_zv, out, sl, sub, _0$$3, _1$$3, _2$$4, _3$$4, _4$$4;
	zend_string *format = NULL;

	ZVAL_UNDEF(&format_zv);
	ZVAL_UNDEF(&out);
	ZVAL_UNDEF(&sl);
	ZVAL_UNDEF(&sub);
	ZVAL_UNDEF(&_0$$3);
	ZVAL_UNDEF(&_1$$3);
	ZVAL_UNDEF(&_2$$4);
	ZVAL_UNDEF(&_3$$4);
	ZVAL_UNDEF(&_4$$4);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(format)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&format_zv);
	ZVAL_STR_COPY(&format_zv, format);
	ZEPHIR_INIT_VAR(&out);
	ZVAL_STRING(&out, "");
	ZEPHIR_INIT_VAR(&sl);
	ZVAL_LONG(&sl, zephir_fast_strlen_ev(&format_zv));
	i = 0;
	while (1) {
		if (!(ZEPHIR_GT_LONG(&sl, i))) {
			break;
		}
		ZVAL_LONG(&_0$$3, i);
		ZVAL_LONG(&_1$$3, 1);
		ZEPHIR_INIT_NVAR(&sub);
		zephir_substr(&sub, &format_zv, zephir_get_intval(&_0$$3), 1 , 0);
		if (ZEPHIR_IS_STRING(&sub, "\\")) {
			kos$$4 = (i + 1);
			ZVAL_LONG(&_2$$4, kos$$4);
			ZVAL_LONG(&_3$$4, 1);
			ZEPHIR_INIT_NVAR(&_4$$4);
			zephir_substr(&_4$$4, &format_zv, zephir_get_intval(&_2$$4), 1 , 0);
			zephir_concat_self(&out, &_4$$4);
			i = (i + 2);
			continue;
		}
		zephir_concat_self(&out, &sub);
		i = (i + 1);
	}
	RETURN_CCTOR(&out);
}

