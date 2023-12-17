
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
	zval r;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&r);
	ZVAL_NULL(&r);
	object_init_ex(&r, zephir_get_internal_ce(SL("reflectionclass")));
	ZEPHIR_CALL_METHOD(NULL, &r, "__construct", NULL, 84, this_ptr);
	zephir_check_call_status();
	RETURN_CCTOR(&r);
}

PHP_METHOD(Stub_Reflection, getReflectionFunction)
{
	zval r, _0;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&r);
	ZVAL_UNDEF(&_0);
	ZVAL_NULL(&r);
	object_init_ex(&r, zephir_get_internal_ce(SL("reflectionfunction")));
	ZVAL_NULL(&_0);
	ZVAL_STRING(&_0, "abs");
	ZEPHIR_CALL_METHOD(NULL, &r, "__construct", NULL, 85, &_0);
	zephir_check_call_status();
	RETURN_CCTOR(&r);
}

PHP_METHOD(Stub_Reflection, setReflectionParameter)
{
	zval *parameter, parameter_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&parameter_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(parameter, zephir_get_internal_ce(SL("reflectionparameter")))
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &parameter);
	zephir_update_property_zval(this_ptr, ZEND_STRL("reflectionParameter"), parameter);
	RETURN_MEMBER(getThis(), "reflectionParameter");
}

PHP_METHOD(Stub_Reflection, getReflectionParameter)
{

	RETURN_MEMBER(getThis(), "reflectionParameter");
}

