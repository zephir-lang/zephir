
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
#include "kernel/operators.h"
#include "kernel/object.h"
#include "kernel/array.h"
#include "kernel/fcall.h"
#include "kernel/concat.h"
#include "kernel/exception.h"
#include "ext/spl/spl_exceptions.h"
#include "kernel/generator.h"


/**
 * A local whose only assignment sits inside a conditional keeps the ZVAL_UNDEF
 * it receives at declaration whenever that branch does not run, and that zval
 * reaches userland: var_dump() renders it as UNKNOWN:0, which is not a PHP
 * value. PHP evaluates an unset variable as null.
 *
 * This is the conditional half of #2654, which covered a local that is never
 * assigned at all.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2679
 */
ZEPHIR_INIT_CLASS(Stub_Issue2679)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2679, stub, issue2679, stub_issue2679_method_entry, 0);

	zend_declare_property_null(stub_issue2679_ce, SL("retained"), ZEND_ACC_PROTECTED);
	stub_issue2679_ce->create_object = zephir_init_properties_Stub_Issue2679;

	return SUCCESS;
}

/**
 * The first shape from the issue: one assignment, inside an if.
 */
PHP_METHOD(Stub_Issue2679, conditional)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *flag_param = NULL, x;
	zend_bool flag;

	ZVAL_UNDEF(&x);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_BOOL(flag)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &flag_param);
	ZEPHIR_INIT_VAR(&x);
	if (flag) {
		ZEPHIR_INIT_NVAR(&x);
		ZVAL_STRING(&x, "set");
	}
	RETURN_CCTOR(&x);
}

/**
 * Guard: `fetch` looks conditional but is not. Every fetch helper writes
 * its target on both paths, the value on a hit and ZVAL_NULL on a miss
 * (kernel/array.c zephir_array_isset_*_fetch, kernel/object.c
 * zephir_fetch_property*), so the target is always a valid zval.
 *
 * It also must not be touched: a read-only fetch hands back a borrowed
 * value with no reference taken, so registering the slot with the memory
 * frame would free a reference this method never owned.
 */
PHP_METHOD(Stub_Issue2679, fetchCase)
{
	zend_bool found;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *data_param = NULL, v, _0;
	zval data;

	ZVAL_UNDEF(&data);
	ZVAL_UNDEF(&v);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(data, data_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &data_param);
	zephir_get_arrval(&data, data_param);
	found = 0;
	zephir_memory_observe(&v);
	if (zephir_array_isset_string_fetch(&v, &data, SL("k"), 0)) {
		found = 1;
	}
	zephir_create_array(return_value, 2, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_BOOL(&_0, found);
	zephir_array_fast_append(return_value, &_0);
	zephir_array_fast_append(return_value, &v);
	RETURN_MM();
}

/**
 * Guard: the fetch target is read only where the fetch succeeded, so it is
 * definitely assigned at every read and needs no initialization.
 */
PHP_METHOD(Stub_Issue2679, fetchGuarded)
{
	zval _0$$3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *data_param = NULL, v;
	zval data;

	ZVAL_UNDEF(&data);
	ZVAL_UNDEF(&v);
	ZVAL_UNDEF(&_0$$3);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(data, data_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &data_param);
	zephir_get_arrval(&data, data_param);
	zephir_memory_observe(&v);
	if (zephir_array_isset_string_fetch(&v, &data, SL("k"), 0)) {
		zephir_cast_to_string(&_0$$3, &v);
		RETURN_CTOR(&_0$$3);
	}
	RETURN_MM_STRING("missing");
}

/**
 * Guard: same as above, as a standalone statement.
 */
PHP_METHOD(Stub_Issue2679, fetchStatement)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *data_param = NULL, v;
	zval data;

	ZVAL_UNDEF(&data);
	ZVAL_UNDEF(&v);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(data, data_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &data_param);
	zephir_get_arrval(&data, data_param);
	zephir_memory_observe(&v);
	zephir_array_isset_string_fetch(&v, &data, SL("k"), 0);
	RETURN_CCTOR(&v);
}

/**
 * Guard: both arms assign, so the variable is definitely assigned after
 * the if and nothing has to be added.
 */
PHP_METHOD(Stub_Issue2679, exhaustiveIfElse)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *flag_param = NULL, x;
	zend_bool flag;

	ZVAL_UNDEF(&x);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_BOOL(flag)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &flag_param);
	ZEPHIR_INIT_VAR(&x);
	if (flag) {
		ZVAL_STRING(&x, "yes");
	} else {
		ZVAL_STRING(&x, "no");
	}
	RETURN_CCTOR(&x);
}

/**
 * Two conditional writes. The compiler's own `conditional-initialization`
 * warning misses this one because it only fires on a single init branch.
 */
PHP_METHOD(Stub_Issue2679, multiBranch)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, *b_param = NULL, x;
	zend_bool a, b;

	ZVAL_UNDEF(&x);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_BOOL(a)
		Z_PARAM_BOOL(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &a_param, &b_param);
	ZEPHIR_INIT_VAR(&x);
	if (a) {
		ZEPHIR_INIT_NVAR(&x);
		ZVAL_STRING(&x, "a");
	}
	if (b) {
		ZEPHIR_INIT_NVAR(&x);
		ZVAL_STRING(&x, "b");
	}
	RETURN_CCTOR(&x);
}

/**
 * Guard: the conditional write is followed by an unconditional one.
 */
PHP_METHOD(Stub_Issue2679, conditionalThenReassign)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *flag_param = NULL, x;
	zend_bool flag;

	ZVAL_UNDEF(&x);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_BOOL(flag)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &flag_param);
	if (flag) {
		ZEPHIR_INIT_VAR(&x);
		ZVAL_STRING(&x, "first");
	}
	ZEPHIR_INIT_NVAR(&x);
	ZVAL_STRING(&x, "second");
	RETURN_CCTOR(&x);
}

/**
 * A loop body may run zero times.
 */
PHP_METHOD(Stub_Issue2679, loopOnly)
{
	zend_bool _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *items_param = NULL, x, item, *_0, _1;
	zval items;

	ZVAL_UNDEF(&items);
	ZVAL_UNDEF(&x);
	ZVAL_UNDEF(&item);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(items, items_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &items_param);
	zephir_get_arrval(&items, items_param);
	ZEPHIR_INIT_VAR(&x);
	zephir_is_iterable(&items, 0, "stub/issue2679.zep", 144);
	if (Z_TYPE_P(&items) == IS_ARRAY) {
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(&items), _0)
		{
			ZEPHIR_INIT_NVAR(&item);
			ZVAL_COPY(&item, _0);
			ZEPHIR_CPY_WRT(&x, &item);
		} ZEND_HASH_FOREACH_END();
	} else {
		ZEPHIR_CALL_METHOD(NULL, &items, "rewind", NULL, 0);
		zephir_check_call_status();
		_2 = 1;
		while (1) {
			if (_2) {
				_2 = 0;
			} else {
				ZEPHIR_CALL_METHOD(NULL, &items, "next", NULL, 0);
				zephir_check_call_status();
			}
			ZEPHIR_CALL_METHOD(&_1, &items, "valid", NULL, 0);
			zephir_check_call_status();
			if (!zend_is_true(&_1)) {
				break;
			}
			ZEPHIR_CALL_METHOD(&item, &items, "current", NULL, 0);
			zephir_check_call_status();
				ZEPHIR_CPY_WRT(&x, &item);
		}
	}
	ZEPHIR_INIT_NVAR(&item);
	RETURN_CCTOR(&x);
}

PHP_METHOD(Stub_Issue2679, whileOnly)
{
	zval _1$$3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *n_param = NULL, x, _0$$3;
	zend_long n, i;

	ZVAL_UNDEF(&x);
	ZVAL_UNDEF(&_0$$3);
	ZVAL_UNDEF(&_1$$3);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &n_param);
	ZEPHIR_INIT_VAR(&x);
	i = 0;
	while (1) {
		if (!(i < n)) {
			break;
		}
		ZEPHIR_INIT_NVAR(&_0$$3);
		ZVAL_LONG(&_0$$3, i);
		ZEPHIR_INIT_NVAR(&_1$$3);
		ZEPHIR_CONCAT_SV(&_1$$3, "iter", &_0$$3);
		ZEPHIR_CPY_WRT(&x, &_1$$3);
		i++;
	}
	RETURN_CCTOR(&x);
}

/**
 * Guard: a `default` clause makes the switch exhaustive.
 */
PHP_METHOD(Stub_Issue2679, switchWithDefault)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *n_param = NULL, x;
	zend_long n;

	ZVAL_UNDEF(&x);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &n_param);
	if (n == 1) { goto zephir_switch_0_clause_0; }
	goto zephir_switch_0_clause_1;
	zephir_switch_0_clause_0: ;
		ZEPHIR_INIT_VAR(&x);
		ZVAL_STRING(&x, "one");
		goto zephir_switch_0_end;
	zephir_switch_0_clause_1: ;
		ZEPHIR_INIT_NVAR(&x);
		ZVAL_STRING(&x, "other");
		goto zephir_switch_0_end;
	zephir_switch_0_end: ;

	RETURN_CCTOR(&x);
}

PHP_METHOD(Stub_Issue2679, switchWithoutDefault)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *n_param = NULL, x;
	zend_long n;

	ZVAL_UNDEF(&x);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &n_param);
	ZEPHIR_INIT_VAR(&x);
	if (n == 1) { goto zephir_switch_0_clause_0; }
	if (n == 2) { goto zephir_switch_0_clause_1; }
	goto zephir_switch_0_end;
	zephir_switch_0_clause_0: ;
		ZEPHIR_INIT_NVAR(&x);
		ZVAL_STRING(&x, "one");
		goto zephir_switch_0_end;
	zephir_switch_0_clause_1: ;
		ZEPHIR_INIT_NVAR(&x);
		ZVAL_STRING(&x, "two");
		goto zephir_switch_0_end;
	zephir_switch_0_end: ;

	RETURN_CCTOR(&x);
}

/**
 * An exception can leave the try block before the assignment runs.
 */
PHP_METHOD(Stub_Issue2679, tryOnly)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *boom_param = NULL, x, _2, _3, _0$$4, _1$$4;
	zend_bool boom;

	ZVAL_UNDEF(&x);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_0$$4);
	ZVAL_UNDEF(&_1$$4);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_BOOL(boom)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &boom_param);
	ZEPHIR_INIT_VAR(&x);

	/* try_start_1: */

		if (boom) {
			ZEPHIR_INIT_VAR(&_0$$4);
			object_init_ex(&_0$$4, spl_ce_RuntimeException);
			ZEPHIR_INIT_VAR(&_1$$4);
			ZVAL_STRING(&_1$$4, "boom");
			ZEPHIR_CALL_METHOD(NULL, &_0$$4, "__construct", NULL, 67, &_1$$4);
			zephir_check_call_status_or_jump(try_end_1);
			zephir_throw_exception_debug(&_0$$4, "stub/issue2679.zep", 206);
			goto try_end_1;

		}
		ZEPHIR_INIT_NVAR(&x);
		ZVAL_STRING(&x, "ok");

	try_end_1:

	if (EG(exception)) {
		ZEPHIR_INIT_VAR(&_2);
		ZVAL_OBJ(&_2, EG(exception));
		Z_ADDREF_P(&_2);
		ZEPHIR_INIT_VAR(&_3);
		if (zephir_is_instance_of(&_2, SL("RuntimeException"))) {
			zend_clear_exception();
			ZEPHIR_CPY_WRT(&_3, &_2);
			boom = 1;
		}
	}
	RETURN_CCTOR(&x);
}

/**
 * The read happens before the only assignment.
 */
PHP_METHOD(Stub_Issue2679, readBeforeWrite)
{
	zval x, t, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&x);
	ZVAL_UNDEF(&t);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&x);
	ZEPHIR_INIT_VAR(&_0);
	zephir_gettype(&_0, &x);
	ZEPHIR_CPY_WRT(&t, &_0);
	ZEPHIR_INIT_NVAR(&x);
	ZVAL_STRING(&x, "set");
	ZEPHIR_INIT_NVAR(&_0);
	zephir_gettype(&_0, &x);
	ZEPHIR_CONCAT_VSV(return_value, &t, "/", &_0);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2679, stringLocal)
{
	zval s;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *flag_param = NULL;
	zend_bool flag;

	ZVAL_UNDEF(&s);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_BOOL(flag)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &flag_param);
	ZEPHIR_INIT_VAR(&s);
	if (flag) {
		ZEPHIR_INIT_NVAR(&s);
		ZVAL_STRING(&s, "set");
	}
	RETURN_CTOR(&s);
}

PHP_METHOD(Stub_Issue2679, arrayLocal)
{
	zval a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *flag_param = NULL, _0$$3, _1$$3;
	zend_bool flag;

	ZVAL_UNDEF(&_0$$3);
	ZVAL_UNDEF(&_1$$3);
	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_BOOL(flag)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &flag_param);
	ZEPHIR_INIT_VAR(&a);
	if (flag) {
		ZEPHIR_INIT_VAR(&_0$$3);
		zephir_create_array(&_0$$3, 3, 0);
		ZEPHIR_INIT_VAR(&_1$$3);
		ZVAL_LONG(&_1$$3, 1);
		zephir_array_fast_append(&_0$$3, &_1$$3);
		ZEPHIR_INIT_NVAR(&_1$$3);
		ZVAL_LONG(&_1$$3, 2);
		zephir_array_fast_append(&_0$$3, &_1$$3);
		ZEPHIR_INIT_NVAR(&_1$$3);
		ZVAL_LONG(&_1$$3, 3);
		zephir_array_fast_append(&_0$$3, &_1$$3);
		ZEPHIR_CPY_WRT(&a, &_0$$3);
	}
	RETURN_CTOR(&a);
}

/**
 * The conditionally assigned local is handed to a function rather than
 * returned, so the null has to be real and not only visible to var_dump().
 */
PHP_METHOD(Stub_Issue2679, passAsArgument)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *flag_param = NULL, x;
	zend_bool flag;

	ZVAL_UNDEF(&x);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_BOOL(flag)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &flag_param);
	ZEPHIR_INIT_VAR(&x);
	if (flag) {
		ZEPHIR_INIT_NVAR(&x);
		ZVAL_STRING(&x, "set");
	}
	zephir_gettype(return_value, &x);
	RETURN_MM();
}

/**
 * A generator step is a resumable function whose locals are saved and
 * restored across yield points, so Method::compile() rewrites its
 * first-init macros to their N forms. The registration added at the
 * declaration has to survive that rewrite intact.
 */
PHP_METHOD(Stub_Issue2679, conditionalInGenerator)
{
	zval *flag_param = NULL;
	zend_bool flag;
	zval *this_ptr = getThis();

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_BOOL(flag)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &flag_param);
	zephir_generator_create(return_value, this_ptr, stub_issue2679_ce, zep_Stub_Issue2679_zephir_gen_step_conditionalInGenerator, 1);
	zephir_generator_slot_set_bool(return_value, 0, (int) flag);
	return;
}

/**
 * A capture reads the local where the closure is created, so an unassigned
 * one has to arrive in the closure as null.
 */
PHP_METHOD(Stub_Issue2679, conditionalCapturedByClosure)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *flag_param = NULL, x, fnc, _0;
	zend_bool flag;

	ZVAL_UNDEF(&x);
	ZVAL_UNDEF(&fnc);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_BOOL(flag)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &flag_param);
	ZEPHIR_INIT_VAR(&x);
	if (flag) {
		ZEPHIR_INIT_NVAR(&x);
		ZVAL_STRING(&x, "set");
	}
	ZEPHIR_INIT_VAR(&_0);
	object_init_ex(&_0, stub_46__closure_ce);
	zephir_update_property_zval(&_0, SL("x"), &x);
	ZEPHIR_INIT_VAR(&fnc);
	zephir_create_closure_bound(&fnc, &_0, NULL, stub_46__closure_ce, SL("__invoke"));
	ZEPHIR_CALL_USER_FUNC(return_value, &fnc);
	zephir_check_call_status();
	RETURN_MM();
}

/**
 * Allocates a heap string inside the conditional branch and drops it on
 * return, so a slot that stopped being registered with the memory frame
 * shows up as growth in leakProbe().
 */
PHP_METHOD(Stub_Issue2679, conditionalHeap)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *flag_param = NULL, x, _2, _0$$3, _1$$3;
	zend_bool flag;

	ZVAL_UNDEF(&x);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_0$$3);
	ZVAL_UNDEF(&_1$$3);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_BOOL(flag)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &flag_param);
	ZEPHIR_INIT_VAR(&x);
	if (flag) {
		ZEPHIR_INIT_VAR(&_0$$3);
		ZVAL_STRING(&_0$$3, "x");
		ZVAL_LONG(&_1$$3, 256);
		ZEPHIR_CALL_FUNCTION(&x, "str_repeat", NULL, 29, &_0$$3, &_1$$3);
		zephir_check_call_status();
	}
	ZEPHIR_INIT_VAR(&_2);
	zephir_gettype(&_2, &x);
	RETURN_CCTOR(&_2);
}

/**
 * Exercises every conditionally assigned local `iterations` times and
 * reports how much memory the run retained. Nothing is allocated inside
 * the measured window, so a non-zero result is a leak.
 */
PHP_METHOD(Stub_Issue2679, leakProbe)
{
	zval _6$$3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zephir_fcall_cache_entry *_0 = NULL, *_2 = NULL, *_3 = NULL, *_5 = NULL, *_8 = NULL, *_9 = NULL, *_10 = NULL, *_11 = NULL, *_12 = NULL, *_13 = NULL, *_14 = NULL;
	zval *iterations_param = NULL, before, after, sink, _1$$3, _4$$3, _7$$3;
	zend_long iterations, ZEPHIR_LAST_CALL_STATUS, i = 0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&before);
	ZVAL_UNDEF(&after);
	ZVAL_UNDEF(&sink);
	ZVAL_UNDEF(&_1$$3);
	ZVAL_UNDEF(&_4$$3);
	ZVAL_UNDEF(&_7$$3);
	ZVAL_UNDEF(&_6$$3);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(iterations)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &iterations_param);
	ZEPHIR_INIT_VAR(&sink);
	ZVAL_NULL(&sink);
	ZEPHIR_CALL_FUNCTION(&before, "memory_get_usage", &_0, 48);
	zephir_check_call_status();
	i = 0;
	while (1) {
		if (!(i < iterations)) {
			break;
		}
		ZVAL_BOOL(&_1$$3, 1);
		ZEPHIR_CALL_METHOD(&sink, this_ptr, "conditional", &_2, 0, &_1$$3);
		zephir_check_call_status();
		ZVAL_BOOL(&_1$$3, 0);
		ZEPHIR_CALL_METHOD(&sink, this_ptr, "conditional", &_2, 0, &_1$$3);
		zephir_check_call_status();
		ZVAL_BOOL(&_1$$3, 1);
		ZEPHIR_CALL_METHOD(&sink, this_ptr, "conditionalheap", &_3, 0, &_1$$3);
		zephir_check_call_status();
		ZVAL_BOOL(&_1$$3, 0);
		ZEPHIR_CALL_METHOD(&sink, this_ptr, "conditionalheap", &_3, 0, &_1$$3);
		zephir_check_call_status();
		ZVAL_BOOL(&_1$$3, 1);
		ZVAL_BOOL(&_4$$3, 1);
		ZEPHIR_CALL_METHOD(&sink, this_ptr, "multibranch", &_5, 0, &_1$$3, &_4$$3);
		zephir_check_call_status();
		ZEPHIR_INIT_NVAR(&_6$$3);
		zephir_create_array(&_6$$3, 2, 0);
		ZEPHIR_INIT_NVAR(&_7$$3);
		ZVAL_LONG(&_7$$3, 1);
		zephir_array_fast_append(&_6$$3, &_7$$3);
		ZEPHIR_INIT_NVAR(&_7$$3);
		ZVAL_LONG(&_7$$3, 2);
		zephir_array_fast_append(&_6$$3, &_7$$3);
		ZEPHIR_CALL_METHOD(&sink, this_ptr, "looponly", &_8, 0, &_6$$3);
		zephir_check_call_status();
		ZVAL_LONG(&_1$$3, 2);
		ZEPHIR_CALL_METHOD(&sink, this_ptr, "whileonly", &_9, 0, &_1$$3);
		zephir_check_call_status();
		ZVAL_LONG(&_1$$3, 1);
		ZEPHIR_CALL_METHOD(&sink, this_ptr, "switchwithoutdefault", &_10, 0, &_1$$3);
		zephir_check_call_status();
		ZVAL_BOOL(&_1$$3, 0);
		ZEPHIR_CALL_METHOD(&sink, this_ptr, "tryonly", &_11, 0, &_1$$3);
		zephir_check_call_status();
		ZVAL_BOOL(&_1$$3, 1);
		ZEPHIR_CALL_METHOD(&sink, this_ptr, "stringlocal", &_12, 0, &_1$$3);
		zephir_check_call_status();
		ZVAL_BOOL(&_1$$3, 1);
		ZEPHIR_CALL_METHOD(&sink, this_ptr, "arraylocal", &_13, 0, &_1$$3);
		zephir_check_call_status();
		ZVAL_BOOL(&_1$$3, 1);
		ZEPHIR_CALL_METHOD(&sink, this_ptr, "passasargument", &_14, 0, &_1$$3);
		zephir_check_call_status();
		i++;
	}
	ZEPHIR_CALL_FUNCTION(&after, "memory_get_usage", &_0, 48);
	zephir_check_call_status();
	zephir_sub_function(return_value, &after, &before);
	RETURN_MM();
}

/**
 * The positive control for leakProbe(): it retains every value it produces,
 * so it must report growth. A control that replaces what it retains reads
 * zero and proves nothing.
 */
PHP_METHOD(Stub_Issue2679, leakProbeControl)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zephir_fcall_cache_entry *_0 = NULL, *_3 = NULL;
	zval *iterations_param = NULL, before, after, _1$$3, _2$$3;
	zend_long iterations, ZEPHIR_LAST_CALL_STATUS, i = 0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&before);
	ZVAL_UNDEF(&after);
	ZVAL_UNDEF(&_1$$3);
	ZVAL_UNDEF(&_2$$3);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(iterations)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &iterations_param);
	ZEPHIR_CALL_FUNCTION(&before, "memory_get_usage", &_0, 48);
	zephir_check_call_status();
	i = 0;
	while (1) {
		if (!(i < iterations)) {
			break;
		}
		ZVAL_BOOL(&_2$$3, 1);
		ZEPHIR_CALL_METHOD(&_1$$3, this_ptr, "conditionalheap", &_3, 0, &_2$$3);
		zephir_check_call_status();
		zephir_update_property_array_append(this_ptr, SL("retained"), &_1$$3);
		i++;
	}
	ZEPHIR_CALL_FUNCTION(&after, "memory_get_usage", &_0, 48);
	zephir_check_call_status();
	zephir_sub_function(return_value, &after, &before);
	RETURN_MM();
}

void zep_Stub_Issue2679_zephir_gen_step_conditionalInGenerator(int ht, zend_execute_data *execute_data, zval *return_value, zval *this_ptr, int return_value_used, zval *zephir_gen_ext )
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *zephir_gen, zephir_gen_sub, x, _0;
	zend_bool flag;
		ZVAL_UNDEF(&zephir_gen_sub);
	ZVAL_UNDEF(&x);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_gen = zephir_gen_ext;
	/* Generator resume dispatch (issue #1849) */
	zephir_generator_slots_ensure(zephir_gen, 3);
	if (Z_TYPE_P(zephir_generator_slot(zephir_gen, 0)) != IS_UNDEF) { flag = (Z_TYPE_P(zephir_generator_slot(zephir_gen, 0)) == IS_TRUE); }
	ZEPHIR_GEN_RESTORE_ZVAL(zephir_gen, 1, &_0);
	ZEPHIR_GEN_RESTORE_ZVAL(zephir_gen, 2, &x);
	switch (zephir_generator_get_state(zephir_gen)) {
		case 1: goto zephir_yield_resume_1;
		case 2: goto zephir_yield_resume_2;
		default: break;
	}
	ZEPHIR_INIT_NVAR(&x);
	if (flag) {
		ZEPHIR_INIT_NVAR(&x);
		ZVAL_STRING(&x, "set");
	}
	zephir_generator_yield(zephir_gen, NULL, &x, 1);
	zephir_generator_slot_set_bool(zephir_gen, 0, (int) flag);
		zephir_generator_slot_set(zephir_gen, 1, &_0);
		zephir_generator_slot_set(zephir_gen, 2, &x);
	ZEPHIR_MM_RESTORE();
	return;
	zephir_yield_resume_1:;
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "after");
	zephir_generator_yield(zephir_gen, NULL, &_0, 2);
	zephir_generator_slot_set_bool(zephir_gen, 0, (int) flag);
		zephir_generator_slot_set(zephir_gen, 1, &_0);
		zephir_generator_slot_set(zephir_gen, 2, &x);
	ZEPHIR_MM_RESTORE();
	return;
	zephir_yield_resume_2:;
	zephir_generator_finish(zephir_gen, NULL);
	ZEPHIR_MM_RESTORE();
}

zend_object *zephir_init_properties_Stub_Issue2679(zend_class_entry *class_type)
{
		zval _0, _1$$3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
		ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1$$3);
	

		ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
		zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	
	{
		zval local_this_ptr, *this_ptr = &local_this_ptr;
		ZEPHIR_CREATE_OBJECT(this_ptr, class_type);
		zephir_read_property_ex(&_0, this_ptr, ZEND_STRL("retained"), PH_NOISY_CC | PH_READONLY);
		if (Z_TYPE_P(&_0) == IS_NULL) {
			ZEPHIR_INIT_VAR(&_1$$3);
			array_init(&_1$$3);
			zephir_update_property_zval_ex(this_ptr, ZEND_STRL("retained"), &_1$$3);
		}
		ZEPHIR_MM_RESTORE();
		return Z_OBJ_P(this_ptr);
	}
}

