
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
#include "kernel/array.h"
#include "kernel/memory.h"
#include "kernel/operators.h"
#include "kernel/fcall.h"


/**
 * `unset this->prop[offset]`.
 *
 * Two codegen defects and one kernel gap met here. The literal offset was
 * written into a temp borrowed from a read-only property fetch, so the
 * zend_string it came to own was never released; the property branch fell
 * through into the generic array-access branch, so the unset was emitted
 * twice and an ArrayAccess property received two offsetUnset() calls; and
 * the kernel stayed silent on containers PHP reports an error for.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2702
 */
ZEPHIR_INIT_CLASS(Stub_Issue2702)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2702, stub, issue2702, stub_issue2702_method_entry, 0);

	/**
	 * Typed, so the default is persistent and non-refcounted and the first
	 * unset really separates. That keeps #2698 and #2699 under this fixture
	 * as well.
	 */
	{
		zval _zc0;
		array_init_size(&_zc0, 4);
		add_assoc_long_ex(&_zc0, SL("a"), 1);
		add_assoc_long_ex(&_zc0, SL("b"), 2);
		add_index_stringl(&_zc0, 3, SL("three"));
		zephir_declare_typed_property(stub_issue2702_ce, SL("removable"), &_zc0, ZEND_ACC_PROTECTED, MAY_BE_ARRAY, NULL, 0);
	}

	zend_declare_property_null(stub_issue2702_ce, SL("data"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(stub_issue2702_ce, SL("retained"), ZEND_ACC_PROTECTED);
	/**
	 * Public so a test can bind a PHP reference to it and prove the unset
	 * reaches the referenced array rather than a copy of it.
	 */
	zend_declare_property_null(stub_issue2702_ce, SL("shared"), ZEND_ACC_PUBLIC);
	/**
	 * Whatever a test wants to unset an offset on: an array, an ArrayAccess
	 * object, a plain object, a string, a scalar, null.
	 */
	zend_declare_property_null(stub_issue2702_ce, SL("container"), ZEND_ACC_PUBLIC);
	stub_issue2702_ce->create_object = zephir_init_properties_Stub_Issue2702;

	return SUCCESS;
}

PHP_METHOD(Stub_Issue2702, getData)
{

	RETURN_MEMBER_TYPED(getThis(), "data", IS_ARRAY);
}

PHP_METHOD(Stub_Issue2702, getRemovable)
{

	RETURN_MEMBER_TYPED(getThis(), "removable", IS_ARRAY);
}

PHP_METHOD(Stub_Issue2702, removeLiteral)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "key_a");
	zephir_unset_property_array(this_ptr, ZEND_STRL("data"), &_0);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Issue2702, removeLongLiteral)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 3);
	zephir_unset_property_array(this_ptr, ZEND_STRL("data"), &_0);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Issue2702, removeByVar)
{
	zval *key, key_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&key_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(key)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &key);
	zephir_unset_property_array(this_ptr, ZEND_STRL("data"), key);
}

/**
 * A native `string` local is held as a zend_string plus a `_zv` zval, so
 * the offset is passed through rather than boxed.
 */
PHP_METHOD(Stub_Issue2702, removeByNativeString)
{
	zval key_zv;
	zend_string *key = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&key_zv);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(key)
	ZEND_PARSE_PARAMETERS_END();
	ZVAL_STR(&key_zv, key);
	zephir_unset_property_array(this_ptr, ZEND_STRL("data"), &key_zv);
}

/**
 * A native `int` local is not a zval at all. Passing it straight to
 * zephir_unset_property_array() did not compile, which is why no fixture
 * reached this shape before.
 */
PHP_METHOD(Stub_Issue2702, removeByNativeInt)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *key_param = NULL, _0;
	zend_long key;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(key)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &key_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, key);
	zephir_unset_property_array(this_ptr, ZEND_STRL("data"), &_0);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Issue2702, removeFromTypedProperty)
{
	zval *key, key_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&key_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(key)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &key);
	zephir_unset_property_array(this_ptr, ZEND_STRL("removable"), key);
}

PHP_METHOD(Stub_Issue2702, removeFromShared)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "k");
	zephir_unset_property_array(this_ptr, ZEND_STRL("shared"), &_0);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Issue2702, unsetContainerLiteral)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "k");
	zephir_unset_property_array(this_ptr, ZEND_STRL("container"), &_0);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Issue2702, unsetContainerLongLiteral)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 3);
	zephir_unset_property_array(this_ptr, ZEND_STRL("container"), &_0);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Issue2702, unsetContainerByVar)
{
	zval *key, key_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&key_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(key)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &key);
	zephir_unset_property_array(this_ptr, ZEND_STRL("container"), key);
}

/**
 * The same statement against a local rather than a property. This is the
 * shape almost every unset in the wild has: it reaches the kernel helpers
 * directly instead of through zephir_unset_property_array(), so it is
 * where reporting the container errors PHP reports has the widest reach.
 */
PHP_METHOD(Stub_Issue2702, unsetLocalLiteral)
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
	zephir_array_unset_string(container, SL("k"), PH_SEPARATE);
	RETVAL_ZVAL(container, 1, 0);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2702, unsetLocalLongLiteral)
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
	zephir_array_unset_long(container, 3, PH_SEPARATE);
	RETVAL_ZVAL(container, 1, 0);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2702, unsetLocalByVar)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *container, container_sub, *key, key_sub;

	ZVAL_UNDEF(&container_sub);
	ZVAL_UNDEF(&key_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(container)
		Z_PARAM_ZVAL(key)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &container, &key);
	ZEPHIR_SEPARATE_PARAM(container);
	zephir_array_unset(container, key, PH_SEPARATE);
	RETVAL_ZVAL(container, 1, 0);
	RETURN_MM();
}

/**
 * The snapshot shape of the issue1259 fixtures: the copy taken before the
 * unset must keep the key, which is only true if the unset separated.
 */
PHP_METHOD(Stub_Issue2702, snapshotAroundLiteralUnset)
{
	zval _0, _1, _2;
	zval ret;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&ret);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("removable", 9, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&ret);
	array_init(&ret);
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 71, PH_NOISY_CC | PH_READONLY);
	zephir_array_append(&ret, &_0, PH_SEPARATE, "stub/issue2702.zep", 144);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "a");
	zephir_unset_property_array(this_ptr, ZEND_STRL("removable"), &_1);
	zephir_read_property_cached(&_2, this_ptr, _zephir_prop_0, 71, PH_NOISY_CC | PH_READONLY);
	zephir_array_append(&ret, &_2, PH_SEPARATE, "stub/issue2702.zep", 146);
	RETURN_CTOR(&ret);
}

PHP_METHOD(Stub_Issue2702, literalProbe)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zephir_fcall_cache_entry *_0 = NULL, *_1 = NULL;
	zval *iterations_param = NULL, before, after;
	zend_long iterations, ZEPHIR_LAST_CALL_STATUS, i = 0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&before);
	ZVAL_UNDEF(&after);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(iterations)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &iterations_param);
	ZEPHIR_CALL_FUNCTION(&before, "memory_get_usage", &_0, 49);
	zephir_check_call_status();
	i = 0;
	while (1) {
		if (!(i < iterations)) {
			break;
		}
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "removeliteral", &_1, 0);
		zephir_check_call_status();
		i++;
	}
	ZEPHIR_CALL_FUNCTION(&after, "memory_get_usage", &_0, 49);
	zephir_check_call_status();
	zephir_sub_function(return_value, &after, &before);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2702, longLiteralProbe)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zephir_fcall_cache_entry *_0 = NULL, *_1 = NULL;
	zval *iterations_param = NULL, before, after;
	zend_long iterations, ZEPHIR_LAST_CALL_STATUS, i = 0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&before);
	ZVAL_UNDEF(&after);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(iterations)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &iterations_param);
	ZEPHIR_CALL_FUNCTION(&before, "memory_get_usage", &_0, 49);
	zephir_check_call_status();
	i = 0;
	while (1) {
		if (!(i < iterations)) {
			break;
		}
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "removelongliteral", &_1, 0);
		zephir_check_call_status();
		i++;
	}
	ZEPHIR_CALL_FUNCTION(&after, "memory_get_usage", &_0, 49);
	zephir_check_call_status();
	zephir_sub_function(return_value, &after, &before);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2702, localArrayProbe)
{
	zval local;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zephir_fcall_cache_entry *_0 = NULL;
	zval *iterations_param = NULL, before, after;
	zend_long iterations, ZEPHIR_LAST_CALL_STATUS, i = 0;

	ZVAL_UNDEF(&before);
	ZVAL_UNDEF(&after);
	ZVAL_UNDEF(&local);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(iterations)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &iterations_param);
	ZEPHIR_INIT_VAR(&local);
	array_init(&local);
	ZEPHIR_CALL_FUNCTION(&before, "memory_get_usage", &_0, 49);
	zephir_check_call_status();
	i = 0;
	while (1) {
		if (!(i < iterations)) {
			break;
		}
		zephir_array_unset_string(&local, SL("key_a"), PH_SEPARATE);
		i++;
	}
	ZEPHIR_CALL_FUNCTION(&after, "memory_get_usage", &_0, 49);
	zephir_check_call_status();
	zephir_sub_function(return_value, &after, &before);
	RETURN_MM();
}

/**
 * The instrument has to be able to report growth, or a flat reading from
 * the probes above means nothing.
 */
PHP_METHOD(Stub_Issue2702, growthProbe)
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
	ZEPHIR_CALL_FUNCTION(&before, "memory_get_usage", &_0, 49);
	zephir_check_call_status();
	i = 0;
	while (1) {
		if (!(i < iterations)) {
			break;
		}
		ZEPHIR_INIT_NVAR(&_1$$3);
		ZVAL_STRING(&_1$$3, "x");
		ZVAL_LONG(&_2$$3, 64);
		ZEPHIR_CALL_FUNCTION(&_3$$3, "str_repeat", &_4, 30, &_1$$3, &_2$$3);
		zephir_check_call_status();
		zephir_update_property_array_append(this_ptr, SL("retained"), &_3$$3);
		i++;
	}
	ZEPHIR_CALL_FUNCTION(&after, "memory_get_usage", &_0, 49);
	zephir_check_call_status();
	zephir_sub_function(return_value, &after, &before);
	RETURN_MM();
}

zend_object *zephir_init_properties_Stub_Issue2702(zend_class_entry *class_type)
{
		zval _1$$3, _5$$5;
	zval _0, _2, _4, _3$$4;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
		ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_3$$4);
	ZVAL_UNDEF(&_1$$3);
	ZVAL_UNDEF(&_5$$5);
	

		ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
		zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	
	{
		zval local_this_ptr, *this_ptr = &local_this_ptr;
		ZEPHIR_CREATE_OBJECT(this_ptr, class_type);
		zephir_read_property_ex(&_0, this_ptr, ZEND_STRL("shared"), PH_NOISY_CC | PH_READONLY);
		if (Z_TYPE_P(&_0) == IS_NULL) {
			ZEPHIR_INIT_VAR(&_1$$3);
			zephir_create_array(&_1$$3, 2, 0);
			add_assoc_long_ex(&_1$$3, SL("k"), 1);
			add_assoc_long_ex(&_1$$3, SL("j"), 2);
			zephir_update_property_zval_ex(this_ptr, ZEND_STRL("shared"), &_1$$3);
		}
		zephir_read_property_ex(&_2, this_ptr, ZEND_STRL("retained"), PH_NOISY_CC | PH_READONLY);
		if (Z_TYPE_P(&_2) == IS_NULL) {
			ZEPHIR_INIT_VAR(&_3$$4);
			array_init(&_3$$4);
			zephir_update_property_zval_ex(this_ptr, ZEND_STRL("retained"), &_3$$4);
		}
		zephir_read_property_ex(&_4, this_ptr, ZEND_STRL("data"), PH_NOISY_CC | PH_READONLY);
		if (Z_TYPE_P(&_4) == IS_NULL) {
			ZEPHIR_INIT_VAR(&_5$$5);
			zephir_create_array(&_5$$5, 3, 0);
			add_assoc_stringl_ex(&_5$$5, SL("key_a"), SL("a"));
			add_assoc_stringl_ex(&_5$$5, SL("key_b"), SL("b"));
			add_index_stringl(&_5$$5, 3, SL("three"));
			zephir_update_property_zval_ex(this_ptr, ZEND_STRL("data"), &_5$$5);
		}
		ZEPHIR_MM_RESTORE();
		return Z_OBJ_P(this_ptr);
	}
}

