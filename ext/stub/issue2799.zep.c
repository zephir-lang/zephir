
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
#include "kernel/array.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"


/**
 * An array property default is declared on the class, as PHP declares it:
 * every object starts from the class's default, shares it until it writes,
 * and no code runs for the property when the object is created. Only an
 * array that reads a constant is still built for each object.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2799
 */
ZEPHIR_INIT_CLASS(Stub_Issue2799)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2799, stub, issue2799, stub_issue2799_method_entry, 0);

	{
		zval _zc0;
		array_init_size(&_zc0, 1);
		zephir_declare_property_array(stub_issue2799_ce, SL("items"), &_zc0, ZEND_ACC_PUBLIC);
	}

	{
		zval _zc0;
		array_init_size(&_zc0, 8);
		add_next_index_long(&_zc0, 1);
		zval _zc1;
		array_init_size(&_zc1, 3);
		add_next_index_long(&_zc1, 2);
		add_next_index_long(&_zc1, 3);
		add_assoc_zval_ex(&_zc0, SL("a"), &_zc1);
		zval _zc2;
		array_init_size(&_zc2, 1);
		add_assoc_zval_ex(&_zc0, SL("e"), &_zc2);
		add_assoc_stringl_ex(&_zc0, SL("s"), SL("str"));
		add_assoc_null_ex(&_zc0, SL("n"));
		add_assoc_double_ex(&_zc0, SL("f"), 1.5);
		add_assoc_bool_ex(&_zc0, SL("b"), 1);
		zephir_declare_property_array(stub_issue2799_ce, SL("literal"), &_zc0, ZEND_ACC_PROTECTED);
	}

	zend_declare_property_null(stub_issue2799_ce, SL("withConst"), ZEND_ACC_PUBLIC);
	{
		zval _zc0;
		array_init_size(&_zc0, 1);
		zephir_declare_typed_property(stub_issue2799_ce, SL("typed"), &_zc0, ZEND_ACC_PUBLIC, MAY_BE_ARRAY, NULL, 0);
	}

	zend_declare_property_long(stub_issue2799_ce, SL("scalar"), 5, ZEND_ACC_PUBLIC);
	stub_issue2799_ce->create_object = zephir_init_properties_Stub_Issue2799;
	zephir_declare_class_constant_string(stub_issue2799_ce, SL("K"), "k");

	{
		zval _zc0;
		array_init_size(&_zc0, 1);
		zephir_declare_class_constant_array(stub_issue2799_ce, SL("EMPTY_LIST"), &_zc0);
	}

	return SUCCESS;
}

PHP_METHOD(Stub_Issue2799, getItems)
{

	RETURN_MEMBER(getThis(), "items");
}

PHP_METHOD(Stub_Issue2799, getLiteral)
{

	RETURN_MEMBER(getThis(), "literal");
}

PHP_METHOD(Stub_Issue2799, getWithConst)
{

	RETURN_MEMBER(getThis(), "withConst");
}

PHP_METHOD(Stub_Issue2799, append)
{
	zval *v, v_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&v_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &v);
	zephir_update_property_array_append(this_ptr, SL("items"), v);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
}

PHP_METHOD(Stub_Issue2799, setItem)
{
	zval *k, k_sub, *v, v_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&v_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(k)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &k, &v);
	zephir_update_property_array(this_ptr, SL("items"), k, v);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
}

PHP_METHOD(Stub_Issue2799, unsetItem)
{
	zval *k, k_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&k_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(k)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &k);
	zephir_unset_property_array(this_ptr, ZEND_STRL("items"), k);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
}

PHP_METHOD(Stub_Issue2799, setKey)
{
	zval *k, k_sub, *v, v_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&k_sub);
	ZVAL_UNDEF(&v_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(k)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &k, &v);
	zephir_update_property_array(this_ptr, SL("literal"), k, v);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
}

PHP_METHOD(Stub_Issue2799, appendNested)
{
	zval *v, v_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&v_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &v);
	zephir_update_property_array_multi(this_ptr, SL("literal"), v, SL("sa"), 3, SL("a"));
	if (UNEXPECTED(EG(exception))) {
		return;
	}
}

PHP_METHOD(Stub_Issue2799, appendNestedEmpty)
{
	zval *v, v_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&v_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &v);
	zephir_update_property_array_multi(this_ptr, SL("literal"), v, SL("sa"), 3, SL("e"));
	if (UNEXPECTED(EG(exception))) {
		return;
	}
}

PHP_METHOD(Stub_Issue2799, unsetKey)
{
	zval *k, k_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&k_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(k)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &k);
	zephir_unset_property_array(this_ptr, ZEND_STRL("literal"), k);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
}

PHP_METHOD(Stub_Issue2799, incrementF)
{
	zval _0, _1, *_2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("literal", 7, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_memory_observe(&_1);
	_2 = zephir_fetch_property_rw(this_ptr, _zephir_prop_0, &_1);
	zephir_array_assign_op(_2, &_0, add_function, SL("s"), 2, SL("f"));
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Issue2799, shift)
{
	zval _0, *_1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("literal", 7, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	zephir_memory_observe(&_0);
	_1 = zephir_fetch_property_write(this_ptr, _zephir_prop_0, &_0);
	ZEPHIR_MAKE_WRITE_REF(_1);
	ZEPHIR_RETURN_CALL_FUNCTION("array_shift", NULL, 3, _1);
	ZEPHIR_UNREF_WRITE(_1);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2799, appendConst)
{
	zval *v, v_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&v_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &v);
	zephir_update_property_array_append(this_ptr, SL("withConst"), v);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
}

PHP_METHOD(Stub_Issue2799, appendTyped)
{
	zval *v, v_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&v_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &v);
	zephir_update_property_array_append(this_ptr, SL("typed"), v);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
}

zend_object *zephir_init_properties_Stub_Issue2799(zend_class_entry *class_type)
{
		zval _1$$3;
	zval _0, _2$$3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
		ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2$$3);
	ZVAL_UNDEF(&_1$$3);
	

		ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
		zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	
	{
		zval local_this_ptr, *this_ptr = &local_this_ptr;
		ZEPHIR_CREATE_OBJECT(this_ptr, class_type);
		zephir_read_property_ex(&_0, this_ptr, ZEND_STRL("withConst"), PH_NOISY_CC | PH_READONLY);
		if (Z_TYPE_P(&_0) == IS_NULL) {
			ZEPHIR_INIT_VAR(&_1$$3);
			zephir_create_array(&_1$$3, 2, 0);
			ZEPHIR_INIT_VAR(&_2$$3);
			ZEPHIR_GET_CONSTANT(&_2$$3, "PHP_INT_SIZE");
			zephir_array_fast_append(&_1$$3, &_2$$3);
			ZEPHIR_INIT_NVAR(&_2$$3);
			ZVAL_STRING(&_2$$3, "k");
			zephir_array_fast_append(&_1$$3, &_2$$3);
			zephir_update_property_zval_ex(this_ptr, ZEND_STRL("withConst"), &_1$$3);
		}
		ZEPHIR_MM_RESTORE();
		return Z_OBJ_P(this_ptr);
	}
}

