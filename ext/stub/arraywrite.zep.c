
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
 * Writing `a[k] = v` raises PHP 8's diagnostics: null and false become
 * arrays (false deprecated since 8.1), a scalar container throws, an object
 * is written through its own handler, the offset is converted with PHP's own
 * deprecations and TypeError, and a full array refuses `[]`. Every write form
 * the compiler emits is here; the test compares each against plain PHP.
 */
ZEPHIR_INIT_CLASS(Stub_ArrayWrite)
{
	ZEPHIR_REGISTER_CLASS(Stub, ArrayWrite, stub, arraywrite, stub_arraywrite_method_entry, 0);

	zend_declare_property_null(stub_arraywrite_ce, SL("p"), ZEND_ACC_PUBLIC);
	zend_declare_property_null(stub_arraywrite_ce, SL("sp"), ZEND_ACC_PUBLIC|ZEND_ACC_STATIC);
	return SUCCESS;
}

PHP_METHOD(Stub_ArrayWrite, write)
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
	zephir_array_update_zval(a, k, v, PH_COPY | PH_SEPARATE);
	RETVAL_ZVAL(a, 1, 0);
	RETURN_MM();
}

PHP_METHOD(Stub_ArrayWrite, writeStringLiteral)
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
	zephir_array_update_string(a, SL("k"), v, PH_COPY | PH_SEPARATE);
	RETVAL_ZVAL(a, 1, 0);
	RETURN_MM();
}

PHP_METHOD(Stub_ArrayWrite, writeIntLiteral)
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
	zephir_array_update_long(a, 1, v, PH_COPY | PH_SEPARATE ZEPHIR_DEBUG_PARAMS_DUMMY);
	RETVAL_ZVAL(a, 1, 0);
	RETURN_MM();
}

PHP_METHOD(Stub_ArrayWrite, writeLong)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long k;
	zval *a, a_sub, *k_param = NULL, *v, v_sub;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&v_sub);
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_ZVAL(a)
		Z_PARAM_LONG(k)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 3, 0, &a, &k_param, &v);
	ZEPHIR_SEPARATE_PARAM(a);
	zephir_array_update_long(a, k, v, PH_COPY | PH_SEPARATE ZEPHIR_DEBUG_PARAMS_DUMMY);
	RETVAL_ZVAL(a, 1, 0);
	RETURN_MM();
}

PHP_METHOD(Stub_ArrayWrite, writeNative)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, *k, k_sub, _0;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(k)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &a, &k);
	ZEPHIR_SEPARATE_PARAM(a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 5);
	zephir_array_update_zval(a, k, &_0, PH_COPY | PH_SEPARATE);
	RETVAL_ZVAL(a, 1, 0);
	RETURN_MM();
}

PHP_METHOD(Stub_ArrayWrite, writeNested)
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
	zephir_array_update_multi(a, v, SL("zz"), 2, i, j);
	RETVAL_ZVAL(a, 1, 0);
	RETURN_MM();
}

PHP_METHOD(Stub_ArrayWrite, append)
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
	zephir_array_append(a, v, PH_SEPARATE, "stub/arraywrite.zep", 60);
	RETVAL_ZVAL(a, 1, 0);
	RETURN_MM();
}

PHP_METHOD(Stub_ArrayWrite, appendNative)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, _0;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a);
	ZEPHIR_SEPARATE_PARAM(a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 5);
	zephir_array_append(a, &_0, PH_SEPARATE, "stub/arraywrite.zep", 67);
	RETVAL_ZVAL(a, 1, 0);
	RETURN_MM();
}

PHP_METHOD(Stub_ArrayWrite, appendNested)
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
	zephir_array_update_multi(a, v, SL("za"), 2, k);
	RETVAL_ZVAL(a, 1, 0);
	RETURN_MM();
}

PHP_METHOD(Stub_ArrayWrite, writeThis)
{
	zval *p, p_sub, *k, k_sub, *v, v_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&p_sub);
	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&v_sub);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("p", 1, 1);
	}

	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_ZVAL(p)
		Z_PARAM_ZVAL(k)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(3, 0, &p, &k, &v);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 18, p);
	zephir_update_property_array(this_ptr, SL("p"), k, v);
	RETURN_MEMBER(getThis(), "p");
}

PHP_METHOD(Stub_ArrayWrite, writeThisNested)
{
	zval *p, p_sub, *i, i_sub, *j, j_sub, *v, v_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&p_sub);
	ZVAL_UNDEF(&i_sub);
	ZVAL_UNDEF(&j_sub);
	ZVAL_UNDEF(&v_sub);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("p", 1, 1);
	}

	ZEND_PARSE_PARAMETERS_START(4, 4)
		Z_PARAM_ZVAL(p)
		Z_PARAM_ZVAL(i)
		Z_PARAM_ZVAL(j)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(4, 0, &p, &i, &j, &v);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 18, p);
	zephir_update_property_array_multi(this_ptr, SL("p"), v, SL("zz"), 2, i, j);
	RETURN_MEMBER(getThis(), "p");
}

PHP_METHOD(Stub_ArrayWrite, appendThis)
{
	zval *p, p_sub, *v, v_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&p_sub);
	ZVAL_UNDEF(&v_sub);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("p", 1, 1);
	}

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(p)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &p, &v);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 18, p);
	zephir_update_property_array_append(this_ptr, SL("p"), v);
	RETURN_MEMBER(getThis(), "p");
}

PHP_METHOD(Stub_ArrayWrite, writeStatic)
{
	zval *p, p_sub, *k, k_sub, *v, v_sub, _0;

	ZVAL_UNDEF(&p_sub);
	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&v_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_ZVAL(p)
		Z_PARAM_ZVAL(k)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(3, 0, &p, &k, &v);
	zephir_update_static_property_ce(stub_arraywrite_ce, ZEND_STRL("sp"), p);
	zephir_update_static_property_array_multi_ce(stub_arraywrite_ce, SL("sp"), v, SL("z"), 1, k);
	zephir_read_static_property_ce(&_0, stub_arraywrite_ce, SL("sp"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTORW(&_0);
}

PHP_METHOD(Stub_ArrayWrite, appendStatic)
{
	zval *p, p_sub, *v, v_sub, _0;

	ZVAL_UNDEF(&p_sub);
	ZVAL_UNDEF(&v_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(p)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &p, &v);
	zephir_update_static_property_ce(stub_arraywrite_ce, ZEND_STRL("sp"), p);
	zephir_update_static_property_array_multi_ce(stub_arraywrite_ce, SL("sp"), v, SL("a"), 1);
	zephir_read_static_property_ce(&_0, stub_arraywrite_ce, SL("sp"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTORW(&_0);
}

PHP_METHOD(Stub_ArrayWrite, concatDeep)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, *i, i_sub, *j, j_sub, *k, k_sub, *v, v_sub;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&i_sub);
	ZVAL_UNDEF(&j_sub);
	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&v_sub);
	ZEND_PARSE_PARAMETERS_START(5, 5)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(i)
		Z_PARAM_ZVAL(j)
		Z_PARAM_ZVAL(k)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 5, 0, &a, &i, &j, &k, &v);
	ZEPHIR_SEPARATE_PARAM(a);
	zephir_array_assign_op(a, v, concat_function, SL("zzz"), 3, i, j, k);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETVAL_ZVAL(a, 1, 0);
	RETURN_MM();
}

PHP_METHOD(Stub_ArrayWrite, writeObject)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *o, o_sub, *k, k_sub, *v, v_sub;

	ZVAL_UNDEF(&o_sub);
	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&v_sub);
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_ZVAL(o)
		Z_PARAM_ZVAL(k)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 3, 0, &o, &k, &v);
	ZEPHIR_SEPARATE_PARAM(o);
	zephir_update_property_array(o, SL("p"), k, v);
	RETVAL_ZVAL(o, 1, 0);
	RETURN_MM();
}

