
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
#include "kernel/time.h"
#include "kernel/object.h"
#include "kernel/memory.h"
#include "kernel/operators.h"


/**
 * `time()` must read the clock PHP `time()` reads (#2739). C `time(NULL)`
 * lags `gettimeofday()` for a few milliseconds after each second boundary.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2739
 */
ZEPHIR_INIT_CLASS(Stub_Issue2739)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2739, stub, issue2739, stub_issue2739_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Issue2739, now)
{

	zephir_time(return_value);
	return;
}

PHP_METHOD(Stub_Issue2739, notBefore)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *timestamp_param = NULL, _0;
	zend_long timestamp;

	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(timestamp)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &timestamp_param);
	ZEPHIR_INIT_VAR(&_0);
	zephir_time(&_0);
	RETURN_MM_BOOL(ZEPHIR_GE_LONG(&_0, timestamp));
}

