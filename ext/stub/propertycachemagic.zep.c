
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
#include "kernel/object.h"
#include "kernel/operators.h"


/**
 * Magic accessors over a backing store. Zephir rejects a *literal* access to
 * an undeclared property, so magic is reached only through the dynamic
 * `this->{name}` path (zephir_read_property_zval / _zval writes) — which the
 * property-access optimization deliberately does NOT touch. These methods
 * pin that magic path so we can prove it stays correct.
 *
 * @see https://github.com/zephir-lang/zephir/issues/1884 (property access)
 */
ZEPHIR_INIT_CLASS(Stub_PropertyCacheMagic)
{
	ZEPHIR_REGISTER_CLASS(Stub, PropertyCacheMagic, stub, propertycachemagic, stub_propertycachemagic_method_entry, 0);

	zend_declare_property_null(stub_propertycachemagic_ce, SL("store"), ZEND_ACC_PROTECTED);
	stub_propertycachemagic_ce->create_object = zephir_init_properties_Stub_PropertyCacheMagic;

	return SUCCESS;
}

PHP_METHOD(Stub_PropertyCacheMagic, __get)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval name_zv, _0, _1, _2;
	zend_string *name = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&name_zv);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("store", 5, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(name)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&name_zv);
	ZVAL_STR_COPY(&name_zv, name);
	ZEPHIR_INIT_VAR(&_0);
	zephir_read_property_cached(&_1, this_ptr, _zephir_prop_0, 93, PH_NOISY_CC | PH_READONLY);
	if (zephir_array_isset_value(&_1, &name_zv)) {
		zephir_read_property_cached(&_2, this_ptr, _zephir_prop_0, 93, PH_NOISY_CC | PH_READONLY);
		ZEPHIR_OBS_NVAR(&_0);
		zephir_array_fetch(&_0, &_2, &name_zv, PH_NOISY, "stub/propertycachemagic.zep", 20);
	} else {
		ZEPHIR_INIT_NVAR(&_0);
		ZVAL_NULL(&_0);
	}
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_PropertyCacheMagic, __set)
{
	zval name_zv, *value, value_sub;
	zend_string *name = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&name_zv);
	ZVAL_UNDEF(&value_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(name)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	value = ZEND_CALL_ARG(execute_data, 2);
	ZVAL_STR(&name_zv, name);
	zephir_update_property_array(this_ptr, SL("store"), &name_zv, value);
}

PHP_METHOD(Stub_PropertyCacheMagic, __isset)
{
	zval name_zv, _0;
	zend_string *name = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&name_zv);
	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("store", 5, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(name)
	ZEND_PARSE_PARAMETERS_END();
	ZVAL_STR(&name_zv, name);
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 93, PH_NOISY_CC | PH_READONLY);
	RETURN_BOOL(zephir_array_isset_value(&_0, &name_zv));
}

PHP_METHOD(Stub_PropertyCacheMagic, __unset)
{
	zval name_zv, _0;
	zend_string *name = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&name_zv);
	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("store", 5, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(name)
	ZEND_PARSE_PARAMETERS_END();
	ZVAL_STR(&name_zv, name);
	zephir_unset_property_array(this_ptr, ZEND_STRL("store"), &name_zv);
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 93, PH_NOISY_CC | PH_READONLY);
	zephir_array_unset(&_0, &name_zv, PH_SEPARATE);
}

PHP_METHOD(Stub_PropertyCacheMagic, readVirtual)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval name_zv, _0;
	zend_string *name = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&name_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(name)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&name_zv);
	ZVAL_STR_COPY(&name_zv, name);
	zephir_memory_observe(&_0);
	zephir_read_property_zval(&_0, this_ptr, &name_zv, PH_NOISY_CC);
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_PropertyCacheMagic, writeVirtual)
{
	zval name_zv, *value, value_sub;
	zend_string *name = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&name_zv);
	ZVAL_UNDEF(&value_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(name)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	value = ZEND_CALL_ARG(execute_data, 2);
	ZVAL_STR(&name_zv, name);
	zephir_update_property_zval_zval(this_ptr, &name_zv, value);
}

zend_object *zephir_init_properties_Stub_PropertyCacheMagic(zend_class_entry *class_type)
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
		zephir_read_property_ex(&_0, this_ptr, ZEND_STRL("store"), PH_NOISY_CC | PH_READONLY);
		if (Z_TYPE_P(&_0) == IS_NULL) {
			ZEPHIR_INIT_VAR(&_1$$3);
			array_init(&_1$$3);
			zephir_update_property_zval_ex(this_ptr, ZEND_STRL("store"), &_1$$3);
		}
		ZEPHIR_MM_RESTORE();
		return Z_OBJ_P(this_ptr);
	}
}

