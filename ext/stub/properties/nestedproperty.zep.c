
#ifdef HAVE_CONFIG_H
#include "../../ext_config.h"
#endif

#include <php.h>
#include "../../php_ext.h"
#include "../../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"
#include "kernel/object.h"
#include "kernel/operators.h"


/**
 * @issue https://github.com/zephir-lang/zephir/issues/2532
 */
ZEPHIR_INIT_CLASS(Stub_Properties_NestedProperty)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Properties, NestedProperty, stub, properties_nestedproperty, stub_properties_nestedproperty_method_entry, 0);

	zend_declare_property_null(stub_properties_nestedproperty_ce, SL("bar"), ZEND_ACC_PUBLIC);
	return SUCCESS;
}

PHP_METHOD(Stub_Properties_NestedProperty, __construct)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("bar", 3, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	object_init_ex(&_0, stub_properties_nestedpropertytarget_ce);
	if (zephir_has_constructor(&_0)) {
		ZEPHIR_CALL_METHOD(NULL, &_0, "__construct", NULL, 0);
		zephir_check_call_status();
	}

	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 81, &_0);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Properties_NestedProperty, setData)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *value_param = NULL, _0, _1;
	zend_long value;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	static zend_string *_zephir_prop_0 = NULL;
	static zend_string *_zephir_prop_1 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("bar", 3, 1);
	}
	if (UNEXPECTED(!_zephir_prop_1)) {
		_zephir_prop_1 = zend_string_init("data", 4, 1);
	}

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(value)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 0, 1, &value_param);
	if (!value_param) {
		value = 0;
	} else {
		}
	ZEPHIR_OBS_NVAR(&_0);
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 81, PH_NOISY_CC);
	ZVAL_UNDEF(&_1);
	ZVAL_LONG(&_1, value);
	zephir_update_property_zval_cached(&_0, _zephir_prop_1, 0, &_1);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Properties_NestedProperty, setName)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval value_zv, _0;
	zend_string *value = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&value_zv);
	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	static zend_string *_zephir_prop_1 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("bar", 3, 1);
	}
	if (UNEXPECTED(!_zephir_prop_1)) {
		_zephir_prop_1 = zend_string_init("name", 4, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(value)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&value_zv);
	ZVAL_STR_COPY(&value_zv, value);
	ZEPHIR_OBS_NVAR(&_0);
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 81, PH_NOISY_CC);
	zephir_update_property_zval_cached(&_0, _zephir_prop_1, 0, &value_zv);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Properties_NestedProperty, getBar)
{

	RETURN_MEMBER(getThis(), "bar");
}

