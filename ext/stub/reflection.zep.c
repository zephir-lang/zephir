
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


ZEPHIR_INIT_CLASS(Stub_Reflection) {

	ZEPHIR_REGISTER_CLASS(Stub, Reflection, stub, reflection, stub_reflection_method_entry, 0);

	zend_declare_property_null(stub_reflection_ce, SL("reflectionParameter"), ZEND_ACC_PRIVATE);

	return SUCCESS;

}

PHP_METHOD(Stub_Reflection, getReflectionClass) {

	zval r;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&r);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&r);
	object_init_ex(&r, zephir_get_internal_ce(SL("reflectionclass")));
	ZEPHIR_CALL_METHOD(NULL, &r, "__construct", NULL, 77, this_ptr);
	zephir_check_call_status();
	RETURN_CCTOR(&r);

}

PHP_METHOD(Stub_Reflection, getReflectionFunction) {

	zval r, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&r);
	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&r);
	object_init_ex(&r, zephir_get_internal_ce(SL("reflectionfunction")));
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "abs");
	ZEPHIR_CALL_METHOD(NULL, &r, "__construct", NULL, 78, &_0);
	zephir_check_call_status();
	RETURN_CCTOR(&r);

}

PHP_METHOD(Stub_Reflection, setReflectionParameter) {

	zval *parameter, parameter_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&parameter_sub);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(parameter)
	ZEND_PARSE_PARAMETERS_END();

#endif


	zephir_fetch_params_without_memory_grow(1, 0, &parameter);



	zephir_update_property_zval(this_ptr, ZEND_STRL("reflectionParameter"), parameter);
	RETURN_MEMBER(getThis(), "reflectionParameter");

}

PHP_METHOD(Stub_Reflection, getReflectionParameter) {

	zval *this_ptr = getThis();



	RETURN_MEMBER(getThis(), "reflectionParameter");

}

