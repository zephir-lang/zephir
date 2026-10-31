
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
#include "kernel/fcall.h"
#include "kernel/memory.h"
#include "kernel/object.h"


/**
 * One method call site, for the test that runs it in two requests whose
 * classes may share an address.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2790
 */
ZEPHIR_INIT_CLASS(Stub_Issue2790)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2790, stub, issue2790, stub_issue2790_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Issue2790, callWho)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *o, o_sub;

	ZVAL_UNDEF(&o_sub);
	ZEND_TLS zephir_method_cache _zephir_call_cache[1];
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(o)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &o);
	ZEPHIR_RETURN_CALL_METHOD_CACHED(o, "who", &_zephir_call_cache[0]);
	zephir_check_call_status();
	RETURN_MM();
}

