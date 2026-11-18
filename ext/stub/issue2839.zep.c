
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
#include "kernel/fcall.h"
#include "kernel/operators.h"
#include "kernel/memory.h"


/**
 * Has no constructor, so it is also a Zephir class to build without one.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2839
 */
ZEPHIR_INIT_CLASS(Stub_Issue2839)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2839, stub, issue2839, stub_issue2839_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Issue2839, build)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval parameters;
	zval className_zv, *parameters_param = NULL;
	zend_string *className = NULL;

	ZVAL_UNDEF(&className_zv);
	ZVAL_UNDEF(&parameters);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(className)
		ZEPHIR_Z_PARAM_ARRAY(parameters, parameters_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	parameters_param = ZEND_CALL_ARG(execute_data, 2);
	zephir_memory_observe(&className_zv);
	ZVAL_STR_COPY(&className_zv, className);
	zephir_get_arrval(&parameters, parameters_param);
	ZEPHIR_LAST_CALL_STATUS = zephir_create_instance_params(return_value, &className_zv, &parameters);
	zephir_check_call_status();
	RETURN_MM();
}

