
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
#include "kernel/array.h"
#include "kernel/operators.h"
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(Stub_Globals_Env)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Globals, Env, stub, globals_env, stub_globals_env_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Globals_Env, read)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *name_param = NULL, _ENV, _0;
	zval name;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&name);
	ZVAL_UNDEF(&_ENV);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(name)
	ZEND_PARSE_PARAMETERS_END();


	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_get_global(&_ENV, SL("_ENV"));
	zephir_fetch_params(1, 1, 0, &name_param);
	zephir_get_strval(&name, name_param);


	ZEPHIR_INIT_VAR(&_0);
	if (zephir_array_isset(&_ENV, &name)) {
		zephir_array_fetch(&_0, &_ENV, &name, PH_NOISY, "stub/globals/env.zep", 8);
	} else {
		ZVAL_BOOL(&_0, 0);
	}
	RETURN_CCTOR(&_0);
}

