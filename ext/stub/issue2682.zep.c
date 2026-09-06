
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
#include "kernel/exception.h"
#include "ext/spl/spl_exceptions.h"
#include "kernel/array.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"


/**
 * A read-only subscript read treats the value as borrowed from the container.
 *
 * That holds for a native array: the kernel hands back a ZVAL_COPY_VALUE() and
 * the hash table keeps the value alive. An ArrayAccess object synthesises the
 * value inside offsetGet() and owns nothing afterwards, so the Z_TRY_DELREF_P()
 * the read-only branch used to apply dropped it to refcount 0 with no owner.
 *
 * The reads under test are shaped so the emitter used to promote the target to
 * read-only: an untyped container parameter, a call on an earlier line than the
 * read (the throw is there for that alone), and a target assigned exactly once.
 * On the fix they emit `, 0)` and plain `PH_NOISY`; before it they emitted
 *
 *     zephir_array_isset_string_fetch(&value, container, SL("namespace"), 1)
 *     zephir_array_fetch_string(&value, container, SL("namespace"), PH_NOISY | PH_READONLY, ...)
 *
 * so if a change to those heuristics stops the promotion firing on the buggy
 * kernel, these methods stop testing anything. nativeArrayStaysReadOnly() is
 * the other side: its container is provably an array, so it must keep the
 * shortcut, and Zephir\Test\CodeGen\ReadOnlyArrayAccessTest pins that in C.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2682
 */
ZEPHIR_INIT_CLASS(Stub_Issue2682)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2682, stub, issue2682, stub_issue2682_method_entry, 0);

	zend_declare_property_null(stub_issue2682_ce, SL("retained"), ZEND_ACC_PROTECTED);
	stub_issue2682_ce->create_object = zephir_init_properties_Stub_Issue2682;

	return SUCCESS;
}

/**
 * `fetch` family, kernel/array.c zephir_array_isset_string_fetch().
 *
 * The container's offsetGet() returns an object whose destructor reports
 * back through the container, so the test can tell "released too early"
 * from "released once, on time" without reading the freed slot.
 *
 * Released early, the copy holds the only reference and dropping it runs
 * the destructor before this method returns true. Owned correctly, the
 * refcount runs 1 -> 2 -> 1 and the destructor waits for the memory frame.
 */
PHP_METHOD(Stub_Issue2682, fetchReleasedEarly)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *container, container_sub, value, copy, _0$$4;

	ZVAL_UNDEF(&container_sub);
	ZVAL_UNDEF(&value);
	ZVAL_UNDEF(&copy);
	ZVAL_UNDEF(&_0$$4);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("destroyed", 9, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(container)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &container);
	if (Z_TYPE_P(container) == IS_NULL) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(spl_ce_RuntimeException, "A container is required", "stub/issue2682.zep", 48);
		return;
	}
	zephir_memory_observe(&value);
	if (zephir_array_isset_string_fetch(&value, container, SL("namespace"), 0)) {
		ZEPHIR_CPY_WRT(&copy, &value);
		ZEPHIR_INIT_NVAR(&copy);
		ZVAL_NULL(&copy);
		zephir_memory_observe(&_0$$4);
		zephir_read_property_cached(&_0$$4, container, _zephir_prop_0, 0, PH_NOISY_CC);
		RETURN_CCTOR(&_0$$4);
	}
	RETURN_MM_BOOL(1);
}

/**
 * The same read with nothing copied out of it: with the value released and
 * no owner left, the allocation is simply lost.
 */
PHP_METHOD(Stub_Issue2682, fetchOnly)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *container, container_sub, value;

	ZVAL_UNDEF(&container_sub);
	ZVAL_UNDEF(&value);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(container)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &container);
	if (Z_TYPE_P(container) == IS_NULL) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(spl_ce_RuntimeException, "A container is required", "stub/issue2682.zep", 70);
		return;
	}
	zephir_memory_observe(&value);
	if (zephir_array_isset_string_fetch(&value, container, SL("namespace"), 0)) {
		RETURN_MM_BOOL(1);
	}
	RETURN_MM_BOOL(0);
}

/**
 * Array-access family, kernel/array.c zephir_array_fetch_string().
 */
PHP_METHOD(Stub_Issue2682, readReleasedEarly)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *container, container_sub, value, copy, _0;

	ZVAL_UNDEF(&container_sub);
	ZVAL_UNDEF(&value);
	ZVAL_UNDEF(&copy);
	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("destroyed", 9, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(container)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &container);
	if (Z_TYPE_P(container) == IS_NULL) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(spl_ce_RuntimeException, "A container is required", "stub/issue2682.zep", 88);
		return;
	}
	zephir_memory_observe(&value);
	zephir_array_fetch_string(&value, container, SL("namespace"), PH_NOISY, "stub/issue2682.zep", 91);
	ZEPHIR_CPY_WRT(&copy, &value);
	ZEPHIR_INIT_NVAR(&copy);
	ZVAL_NULL(&copy);
	zephir_memory_observe(&_0);
	zephir_read_property_cached(&_0, container, _zephir_prop_0, 0, PH_NOISY_CC);
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_Issue2682, plainRead)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *container, container_sub, value;

	ZVAL_UNDEF(&container_sub);
	ZVAL_UNDEF(&value);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(container)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &container);
	if (Z_TYPE_P(container) == IS_NULL) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(spl_ce_RuntimeException, "A container is required", "stub/issue2682.zep", 103);
		return;
	}
	zephir_memory_observe(&value);
	zephir_array_fetch_string(&value, container, SL("namespace"), PH_NOISY, "stub/issue2682.zep", 106);
	RETURN_MM_BOOL(Z_TYPE_P(&value) != IS_NULL);
}

/**
 * Both families, `iterations` times. The container's offsetGet() returns a
 * fresh 64 byte string, so an unreleased result shows up as growth.
 * Nothing in the loop retains anything.
 */
PHP_METHOD(Stub_Issue2682, leakProbe)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zephir_fcall_cache_entry *_0 = NULL, *_1 = NULL, *_2 = NULL;
	zend_long iterations, ZEPHIR_LAST_CALL_STATUS, i = 0;
	zval *container, container_sub, *iterations_param = NULL, before, after, sink;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&container_sub);
	ZVAL_UNDEF(&before);
	ZVAL_UNDEF(&after);
	ZVAL_UNDEF(&sink);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(container)
		Z_PARAM_LONG(iterations)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &container, &iterations_param);
	ZEPHIR_INIT_VAR(&sink);
	ZVAL_NULL(&sink);
	ZEPHIR_CALL_FUNCTION(&before, "memory_get_usage", &_0, 48);
	zephir_check_call_status();
	i = 0;
	while (1) {
		if (!(i < iterations)) {
			break;
		}
		ZEPHIR_CALL_METHOD(&sink, this_ptr, "fetchonly", &_1, 0, container);
		zephir_check_call_status();
		ZEPHIR_CALL_METHOD(&sink, this_ptr, "plainread", &_2, 0, container);
		zephir_check_call_status();
		i++;
	}
	ZEPHIR_CALL_FUNCTION(&after, "memory_get_usage", &_0, 48);
	zephir_check_call_status();
	zephir_sub_function(return_value, &after, &before);
	RETURN_MM();
}

/**
 * The positive control for the probe above: it retains what it allocates,
 * so it must report growth. Without it a flat reading would also be what a
 * probe that cannot measure at all reports.
 */
PHP_METHOD(Stub_Issue2682, leakProbeControl)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zephir_fcall_cache_entry *_0 = NULL, *_4 = NULL;
	zval *iterations_param = NULL, before, after, _1$$3, _2$$3, _3$$3;
	zend_long iterations, ZEPHIR_LAST_CALL_STATUS, i = 0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&before);
	ZVAL_UNDEF(&after);
	ZVAL_UNDEF(&_1$$3);
	ZVAL_UNDEF(&_2$$3);
	ZVAL_UNDEF(&_3$$3);
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
		ZEPHIR_INIT_NVAR(&_1$$3);
		ZVAL_STRING(&_1$$3, "x");
		ZVAL_LONG(&_2$$3, 64);
		ZEPHIR_CALL_FUNCTION(&_3$$3, "str_repeat", &_4, 29, &_1$$3, &_2$$3);
		zephir_check_call_status();
		zephir_update_property_array_append(this_ptr, SL("retained"), &_3$$3);
		i++;
	}
	ZEPHIR_CALL_FUNCTION(&after, "memory_get_usage", &_0, 48);
	zephir_check_call_status();
	zephir_sub_function(return_value, &after, &before);
	RETURN_MM();
}

/**
 * The read-only shortcut has to survive for a container that provably holds
 * a native array, which is where it pays for itself. Assigned from a literal
 * and never aliased, so the generated C must keep PH_READONLY here.
 */
PHP_METHOD(Stub_Issue2682, nativeArrayStaysReadOnly)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *mode_param = NULL, table, value;
	zend_long mode;

	ZVAL_UNDEF(&table);
	ZVAL_UNDEF(&value);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(mode)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &mode_param);
	if (mode < 0) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(spl_ce_RuntimeException, "Not reachable, it only moves the last call line", "stub/issue2682.zep", 169);
		return;
	}
	ZEPHIR_INIT_VAR(&table);
	zephir_create_array(&table, 1, 0);
	add_assoc_stringl_ex(&table, SL("namespace"), SL("kept"));
	zephir_array_fetch_string(&value, &table, SL("namespace"), PH_NOISY | PH_READONLY, "stub/issue2682.zep", 173);
	RETURN_CTOR(&value);
}

zend_object *zephir_init_properties_Stub_Issue2682(zend_class_entry *class_type)
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

