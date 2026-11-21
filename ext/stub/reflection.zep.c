
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
#include "kernel/fcall.h"
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(Stub_Reflection)
{
	ZEPHIR_REGISTER_CLASS(Stub, Reflection, stub, reflection, stub_reflection_method_entry, 0);

	zend_declare_property_null(stub_reflection_ce, SL("reflectionParameter"), ZEND_ACC_PRIVATE);
	return SUCCESS;
}

PHP_METHOD(Stub_Reflection, getReflectionClass)
{
	zval r, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&r);
	ZVAL_UNDEF(&_0);
	ZEND_TLS zephir_method_cache _zephir_call_cache[1];
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	object_init_ex(&_0, zephir_get_internal_ce(SL("reflectionclass")));
	zephir_constructor_start(&_0);
	ZEPHIR_CALL_METHOD_CACHED(NULL, &_0, "__construct", &_zephir_call_cache[0], this_ptr);
	zephir_check_call_status();
	zephir_constructor_finish(&_0);
	ZEPHIR_CPY_WRT(&r, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	RETURN_CCTOR(&r);
}

PHP_METHOD(Stub_Reflection, getReflectionFunction)
{
	zval r, _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&r);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_TLS zephir_method_cache _zephir_call_cache[1];
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	object_init_ex(&_0, zephir_get_internal_ce(SL("reflectionfunction")));
	zephir_constructor_start(&_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "abs");
	ZEPHIR_CALL_METHOD_CACHED(NULL, &_0, "__construct", &_zephir_call_cache[0], &_1);
	zephir_check_call_status();
	zephir_constructor_finish(&_0);
	ZEPHIR_CPY_WRT(&r, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	RETURN_CCTOR(&r);
}

PHP_METHOD(Stub_Reflection, setReflectionParameter)
{
	zval *parameter, parameter_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&parameter_sub);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("reflectionParameter", 19, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(parameter, zephir_get_internal_ce(SL("reflectionparameter")))
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &parameter);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 127, parameter);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	RETURN_MEMBER(getThis(), "reflectionParameter");
}

PHP_METHOD(Stub_Reflection, getReflectionParameter)
{

	RETURN_MEMBER(getThis(), "reflectionParameter");
}

